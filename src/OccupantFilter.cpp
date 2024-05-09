////////////////////////////////////////////////////////////////////////////
//
// This file is part of SC4Historical, a DLL Plugin for SimCity 4 that adds
// Batch make historical.
//
// Copyright (c) 2024 Raphael Cheung
//
// This file is licensed under terms of the MIT License.
// See LICENSE.txt for more information.
//
////////////////////////////////////////////////////////////////////////////

#include "OccupantFilter.h"
#include "cIGZVariant.h"
#include "cISC4Occupant.h"
#include "cISCProperty.h"
#include "cISCPropertyHolder.h"

static const uint32_t kOccupantType_Building = 0x278128A0;
static const uint32_t kBuildingPurpose = 0x27812833;
static const uint32_t kBuildingCapacity = 0x27812834;
static const uint32_t kBuildingWealth = 0x27812832;


bool GetBuildingWealth(cISCPropertyHolder* pProperties, Historical::BuildingType buildingType, Historical::BuildingWealth& wealth)
{
	bool result = false;

	if (pProperties)
	{

		const cISCProperty* property = pProperties->GetProperty(kBuildingWealth);

		if (property)
		{
			const cIGZVariant* data = property->GetPropertyValue();

			if (data)
			{
				uint16_t type = data->GetType();

				if (type == cIGZVariant::Type::Uint8 || type == cIGZVariant::Type::Uint8Array)
				{
					wealth =  Historical::MapWealthType(buildingType, data->GetValUint8());
					result = true;
				}
			}
		}
	}

	return result;
}

bool GetBuildingCapacity(cISCPropertyHolder* pProperties, Historical::BuildingWealth wealth, uint32_t& capacity)
{
	bool result = false;

	if (pProperties)
	{
		const cISCProperty* property = pProperties->GetProperty(kBuildingCapacity);

		if (property)
		{
			const cIGZVariant* data = property->GetPropertyValue();

			if (data)
			{
				uint16_t type = data->GetType();
				bool isHit = false;

				if (type == cIGZVariant::Type::Uint32Array)
				{
					uint32_t* arrayCapacity = data->RefUint32();

					for (int i = 0; i < data->GetCount(); i++)
					{
						if (!isHit)
						{
							if (wealth == static_cast<Historical::BuildingWealth>(arrayCapacity[i]))
								isHit = true;
						}
						else
						{
							capacity = arrayCapacity[i];
							result = true;
							break;
						}
					}

				}
			}
		}
	}

	return result;
}

bool GetBuildingPurposeType(cISCPropertyHolder* pProperties, Historical::SC4BuildingPurposeType& purposeType)
{
	bool result = false;

	if (pProperties)
	{
		const cISCProperty* property = pProperties->GetProperty(kBuildingPurpose);

		if (property)
		{
			const cIGZVariant* data = property->GetPropertyValue();

			if (data)
			{
				uint16_t type = data->GetType();

				if (type == cIGZVariant::Type::Uint8 || type == cIGZVariant::Type::Uint8Array)
				{
					purposeType = static_cast<Historical::SC4BuildingPurposeType>(data->GetValUint8());
					result = true;
				}
			}
		}
	}

	return result;
}


OccupantFilter::OccupantFilter(Historical::BuildingType buildingType, Historical::SC4BuildingPurposeType purposeType, uint32_t minCapacity)
	: refCount(0),
	requestedPurposeType(purposeType),
	requestBuildingType(buildingType),
	requestMinCapacity(minCapacity)
{
}

bool OccupantFilter::QueryInterface(uint32_t riid, void** ppvObj)
{
	if (riid == GZIID_cISC4OccupantFilter)
	{
		*ppvObj = static_cast<cISC4OccupantFilter*>(this);
		AddRef();

		return true;
	}
	else if (riid == GZIID_cIGZUnknown)
	{
		*ppvObj = static_cast<cIGZUnknown*>(this);
		AddRef();

		return true;
	}

	return false;
}

uint32_t OccupantFilter::AddRef()
{
	return ++refCount;
}

uint32_t OccupantFilter::Release()
{
	if (refCount > 0)
	{
		--refCount;
	}

	return refCount;
}

bool OccupantFilter::IsOccupantIncluded(cISC4Occupant* pOccupant)
{
	//Used to test the lot with the specified name
	//const cISCProperty* property = pOccupant->AsPropertyHolder()->GetProperty(0x20);
	//if (property != NULL)
	//{
	//	const cIGZVariant* data = property->GetPropertyValue();
	//	if (data != NULL)
	//	{
	//		char* str = data->RefRZChar();
	//		if (strcmp(str, "Low_Income_Apartments") == 0)
	//			int a = 0;
	//	}
	//}

	return IsOccupantTypeIncluded(pOccupant->GetType())
		&& IsCompatibleBuildingPurpose(pOccupant->AsPropertyHolder())
		&& IsCompatibleCapacity(pOccupant->AsPropertyHolder());
}

bool OccupantFilter::IsOccupantTypeIncluded(uint32_t dwType)
{
	return dwType == kOccupantType_Building;
}


bool OccupantFilter::IsPropertyHolderIncluded(cISCPropertyHolder* pProperties)
{
	return true;
}

bool OccupantFilter::IsCompatibleBuildingPurpose(cISCPropertyHolder* pProperties) const
{
	bool result = false;

	Historical::SC4BuildingPurposeType purposeType = Historical::SC4BuildingPurposeType::None;

	if (GetBuildingPurposeType(pProperties, purposeType))
	{
		result = requestedPurposeType == purposeType;
	}

	return result;
}

bool OccupantFilter::IsCompatibleCapacity(cISCPropertyHolder* pProperties) const
{
	bool result = false;
	Historical::BuildingWealth wealth;
	uint32_t capacity = 0;

	if (GetBuildingWealth(pProperties, requestBuildingType, wealth) && GetBuildingCapacity(pProperties, wealth, capacity))
	{
		result = capacity >= requestMinCapacity;
	}

	return result;
}