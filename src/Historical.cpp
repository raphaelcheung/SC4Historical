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
#include "cRZBaseString.h"
#include "SC4NotificationDialog.h"
#include "StringViewUtil.h"
#include <vector>

using namespace std::string_view_literals;


namespace Historical
{
	void ShowMessageBox(const char* const message)
	{
		SC4NotificationDialog::ShowDialog(cRZBaseString(message), cRZBaseString("Historical"));
	}

	void ShowUsage()
	{
		ShowMessageBox("Usage: his <building type> <min capacity> [make historical]");
	}

	SC4BuildingPurposeType MapBuildingType(BuildingType type)
	{
		SC4BuildingPurposeType result = SC4BuildingPurposeType::Residence;

		switch (type)
		{
		case BuildingType::Office: result = SC4BuildingPurposeType::Office; break;
		case BuildingType::Service: result = SC4BuildingPurposeType::Services; break;
		case BuildingType::Industry: result = SC4BuildingPurposeType::Manufacturing; break;
		}

		return result;
	}

	bool ParseBuildingType(
		const std::string_view& str,
		BuildingType& buildingType)
	{
		bool result = false;

		if (StringViewUtil::StartsWithIgnoreCase(str, "r"sv))
		{
			buildingType = BuildingType::Residence;
			result = true;
		}
		else if (StringViewUtil::StartsWithIgnoreCase(str, "o"sv))
		{
			buildingType = BuildingType::Office;
			result = true;
		}
		else if (StringViewUtil::StartsWithIgnoreCase(str, "s"sv))
		{
			buildingType = BuildingType::Service;
			result = true;
		}
		else if (StringViewUtil::StartsWithIgnoreCase(str, "i"sv))
		{
			buildingType = BuildingType::Industry;
			result = true;
		}

		return result;
	}

	bool ParseMakeHistoricalString(const std::string_view& makeHistorical, bool& value)
	{
		bool result = false;

		if (StringViewUtil::EqualsIgnoreCase(makeHistorical, "true"sv))
		{
			value = true;
			result = true;
		}
		else if (StringViewUtil::EqualsIgnoreCase(makeHistorical, "false"sv))
		{
			value = false;
			result = true;
		}
		else if (makeHistorical.size() == 1)
		{
			switch (makeHistorical[0])
			{
			case '0':
				value = false;
				result = true;
				break;
			case '1':
				value = true;
				result = true;
				break;
			}
		}

		return result;
	}

	bool ParseCheatString(const std::string_view& cheatString, BuildingData& data)
	{
		data.buildingType = BuildingType::Residence;
		data.sc4BuildingType = SC4BuildingPurposeType::Residence;
		data.makeLotHistorical = true;

		std::vector<std::string_view> arguments;
		arguments.reserve(4); // A valid cheat will have at most 4 parameters.

		StringViewUtil::Split(cheatString, ' ', arguments);
		if (arguments.size() >= 2 && arguments.size() <= 5)
		{
			if (!ParseBuildingType(arguments[1], data.buildingType))
			{
				ShowMessageBox("The building type value must be one of: R, O, S, I.");

				return false;
			}

			if (arguments.size() >= 3)
			{
				int tmp = atoi(arguments[2].data());
				data.minCapacityLimit = tmp < 0 ? 0 : tmp;


				if (arguments.size() == 4)
				{
					if (!ParseMakeHistoricalString(arguments[3], data.makeLotHistorical))
					{
						ShowMessageBox("The historical value must be true, false.");
						return false;
					}
				}
			}

			data.sc4BuildingType = MapBuildingType(data.buildingType);
			return true;
		}


		ShowUsage();
		return false;
	}

	void ShowConvertedLotCount(int32_t count)
	{
		char buffer[1024]{};

		std::snprintf(
			buffer,
			sizeof(buffer),
			"Made %d lot(s).",
			count);

		ShowMessageBox(buffer);

	}

	BuildingWealth MapWealthType(BuildingType buildingType, uint32_t wealth)
	{
		switch (wealth)
		{
		case 1:
			switch (buildingType)
			{
			case BuildingType::Residence: return BuildingWealth::R$;
			case BuildingType::Service: return BuildingWealth::CS$;
			case BuildingType::Industry: return BuildingWealth::ID$;
			}
			break;
		case 2:
			switch (buildingType)
			{
			case BuildingType::Residence: return BuildingWealth::R$$;
			case BuildingType::Service: return BuildingWealth::CS$$;
			case BuildingType::Industry: return BuildingWealth::ID$$;
			case BuildingType::Office: return BuildingWealth::CO$$;
			}
			break;
		case 3:
			switch (buildingType)
			{
			case BuildingType::Residence: return BuildingWealth::R$$$;
			case BuildingType::Service: return BuildingWealth::CS$$$;
			case BuildingType::Industry: return BuildingWealth::ID$$$;
			case BuildingType::Office: return BuildingWealth::CO$$$;
			}
			break;
		}

		return BuildingWealth::INVALID;
	}
}