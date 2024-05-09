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

#pragma once
#include "cISC4OccupantFilter.h"
#include "Historical.h"

class OccupantFilter final : public cISC4OccupantFilter
{
public:
	OccupantFilter(Historical::BuildingType buildingType, Historical::SC4BuildingPurposeType purposeType, uint32_t minCapacity);

private:

	bool QueryInterface(uint32_t riid, void** ppvObj) override;

	uint32_t AddRef() override;

	uint32_t Release() override;

	bool IsOccupantIncluded(cISC4Occupant* pOccupant) override;

	bool IsOccupantTypeIncluded(uint32_t dwType) override;

	bool IsPropertyHolderIncluded(cISCPropertyHolder* pProperties) override;

	bool IsCompatibleBuildingPurpose(cISCPropertyHolder* pProperties) const;
	bool IsCompatibleCapacity(cISCPropertyHolder* pProperties) const;

	uint32_t refCount;
	Historical::SC4BuildingPurposeType requestedPurposeType;
	Historical::BuildingType requestBuildingType;
	uint32_t requestMinCapacity;
};