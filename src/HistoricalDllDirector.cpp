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

#include "Historical.h"
#include "OccupantFilter.h"
#include "Logger.h"
#include "StringViewUtil.h"
#include "cIGZApp.h"
#include "cIGZCheatCodeManager.h"
#include "cIGZCOM.h"
#include "cIGZFrameWork.h"
#include "cIGZMessageServer2.h"
#include "cIGZMessage2Standard.h"
#include "cIGZVariant.h"
#include "cRZAutoRefCount.h"
#include "cRZBaseString.h"
#include "cRZMessage2COMDirector.h"
#include "cISC4App.h"
#include "cISC4City.h"
#include "cISC4Lot.h"
#include "cISC4LotManager.h"
#include "cISC4Occupant.h"
#include "cISC4OccupantManager.h"
#include "cISC4SimGrid.h"
#include "cISC4ZoneManager.h"
#include "cISCProperty.h"
#include "cISCPropertyHolder.h"
#include "GZServPtrs.h"
#include "SC4Rect.h"
#include "StringResourceKey.h"
#include "StringResourceManager.h"

static constexpr uint32_t kSC4MessagePostCityInit = 0x26D31EC1;
static constexpr uint32_t kSC4MessagePreCityShutdown = 0x26D31EC2;
static constexpr uint32_t kMessageCheatIssued = 0x230E27AC;

static constexpr uint32_t kHistoricalCheatID = 0x6AABEBA2;

static constexpr uint32_t kHistoricalDirectorID = 0x8E9901E9;

static const uint32_t kOccupantType_Building = 0x278128A0;


static constexpr std::string_view PluginLogFileName = "SC4Historical.log";


bool GetBuildingPurposeType(cISCPropertyHolder* pProperties, Historical::SC4BuildingPurposeType& purposeType);

static cIGZString* GetOccupantName(cISC4Occupant* pOccupant)
{
	cIGZString* name = nullptr;

	cISCPropertyHolder* propertyHolder = pOccupant->AsPropertyHolder();

	constexpr uint32_t kUserVisibleName = 0x8A416A99;

	cISCProperty* userVisibleName = propertyHolder->GetProperty(kUserVisibleName);

	if (userVisibleName)
	{
		const cIGZVariant* propertyValue = userVisibleName->GetPropertyValue();

		if (propertyValue->GetType() == cIGZVariant::Type::Uint32Array
			&& propertyValue->GetCount() == 3)
		{
			const uint32_t* pTGI = propertyValue->RefUint32();

			uint32_t group = pTGI[1];
			uint32_t instance = pTGI[2];

			StringResourceKey key(group, instance);

			StringResourceManager::GetLocalizedString(key, &name);
		}
	}

	return name;
}


struct BuildingIteratorContext
{
	cISC4LotManager* pLotManager;
	cISC4ZoneManager* pZoneManager;

	int32_t convertedLotCount;
	bool makeHistorical;
	Historical::SC4BuildingPurposeType requestType;
	Historical::BuildingType requestBuildingType;
	uint32_t minCapacityLimit;
};

static bool BuildingIterator(cISC4Occupant* pOccupant, void* pData)
{
	BuildingIteratorContext* pContext = static_cast<BuildingIteratorContext*>(pData);

	cISC4Lot* pLot = pContext->pLotManager->GetOccupantLot(pOccupant);

	if (pLot)
	{
		pLot->SetHistorical(pContext->makeHistorical);
		pContext->convertedLotCount++;
	}

	return true;
}

class HistoricalDllDirector : public cRZMessage2COMDirector
{
public:

	HistoricalDllDirector()
	{

	}

	uint32_t GetDirectorID() const
	{
		return kHistoricalDirectorID;
	}

	void PostCityInit()
	{

		cISC4AppPtr pSC4App;

		if (pSC4App)
		{
			cIGZCheatCodeManager* pCheatMgr = pSC4App->GetCheatCodeManager();

			if (pCheatMgr)
			{
				pCheatMgr->AddNotification2(this, 0);
				pCheatMgr->RegisterCheatCode(kHistoricalCheatID, cRZBaseString("his"));
			}
			else
			{
			}
		}
	}

	void PreCityShutdown()
	{
		cISC4AppPtr pSC4App;

		if (pSC4App)
		{
			cIGZCheatCodeManager* pCheatMgr = pSC4App->GetCheatCodeManager();

			if (pCheatMgr)
			{
				pCheatMgr->UnregisterCheatCode(kHistoricalCheatID);
				pCheatMgr->RemoveNotification2(this, 0);
			}
		}
	}

	void ProcessCheat(cIGZMessage2Standard* pStandardMsg)
	{
		uint32_t cheatID = static_cast<uint32_t>(pStandardMsg->GetData1());

		if (cheatID == kHistoricalCheatID)
		{
			cIGZString* cheatStr = static_cast<cIGZString*>(pStandardMsg->GetVoid2());

			Historical::BuildingData data{};

			if (Historical::ParseCheatString(cheatStr->ToChar(), data))
			{
				cISC4AppPtr pSC4App;

				if (pSC4App)
				{
					cISC4City* pCity = pSC4App->GetCity();

					if (pCity)
					{
						cISC4LotManager* pLotManager = pCity->GetLotManager();
						cISC4OccupantManager* pOccupantManager = pCity->GetOccupantManager();
						cISC4ZoneManager* pZoneManager = pCity->GetZoneManager();

						if (pLotManager && pOccupantManager && pZoneManager)
						{
							BuildingIteratorContext context{};
							context.pLotManager = pLotManager;
							context.pZoneManager = pZoneManager;
							context.requestType = data.sc4BuildingType;
							context.minCapacityLimit = data.minCapacityLimit;
							context.makeHistorical = data.makeLotHistorical;
							context.convertedLotCount = 0;
							context.requestBuildingType = data.buildingType;

							OccupantFilter filter(context.requestBuildingType, data.sc4BuildingType, context.minCapacityLimit);

							pOccupantManager->IterateOccupants(&BuildingIterator, &context, nullptr, nullptr, &filter);

							Historical::ShowConvertedLotCount(context.convertedLotCount);
						}
					}
				}
			}
		}
	}

	bool DoMessage(cIGZMessage2* pMessage)
	{
		cIGZMessage2Standard* pStandardMsg = static_cast<cIGZMessage2Standard*>(pMessage);

		uint32_t dwType = pMessage->GetType();

		switch (dwType)
		{
		case kMessageCheatIssued:
			ProcessCheat(pStandardMsg);
			break;
		case kSC4MessagePostCityInit:
			PostCityInit();
			break;
		case kSC4MessagePreCityShutdown:
			PreCityShutdown();
			break;
		}

		return true;
	}

	bool OnStart(cIGZCOM* pCOM)
	{
		cIGZFrameWork* const pFramework = pCOM->FrameWork();

		if (pFramework->GetState() < cIGZFrameWork::kStatePreAppInit)
		{
			pFramework->AddHook(this);
		}
		else
		{
			PreAppInit();
		}

		return true;
	}

	bool PostAppInit()
	{

		cIGZMessageServer2Ptr pMsgServ;

		if (pMsgServ)
		{
			std::vector<uint32_t> requiredNotifications;
			requiredNotifications.push_back(kSC4MessagePostCityInit);
			requiredNotifications.push_back(kSC4MessagePreCityShutdown);

			for (uint32_t messageID : requiredNotifications)
			{
				if (!pMsgServ->AddNotification(this, messageID))
				{
					return false;
				}
			}
		}
		else
		{
			return false;
		}

		return true;
	}
};

cRZCOMDllDirector* RZGetCOMDllDirector() {
	static HistoricalDllDirector sDirector;
	return &sDirector;
}