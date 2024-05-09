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
#include <string_view>


namespace Historical
{
	enum class BuildingType
	{
		Residence,  //住房
		Office,		//办公室
		Service,	//店铺
		Industry	//厂房
	};

	enum class BuildingWealth : uint32_t
	{
		INVALID = 0,
		R$ = 0x1010,
		R$$ = 0x1020,
		R$$$ = 0x1030,
		CS$ = 0x3110,
		CS$$ = 0x3120,
		CS$$$ = 0x3130,
		CO$$ = 0x3320,
		CO$$$ = 0x3330,
		ID$ = 0x4200,			//Industry Dirty
		ID$$ = 0x4300,			//Industry Manufacturing
		ID$$$ = 0x4400,			//Industry High-Tech
	};

	enum class SC4BuildingPurposeType : uint8_t
	{
		None = 0,
		Residence = 1,
		Services = 2,
		Office = 3,
		Tourism = 4,
		Agriculture = 5,
		Processing = 6,
		Manufacturing = 7,
		HighTech = 8,
		Other = 9,
	};

	enum class SC4ZoneType : uint32_t
	{
		None = 0,
		ResidentialLowDensity = 1,
		ResidentialMediumDensity = 2,
		ResidentialHighDensity = 3,
		CommercialLowDensity = 4,
		CommercialMediumDensity = 5,
		CommercialHighDensity = 6,
		// Agriculture is the low density industrial zone.
		Agriculture = 7,
		IndustrialMediumDensity = 8,
		IndustrialHighDensity = 9,
		Military = 10,
		Airport = 11,
		Seaport = 12,
		Spaceport = 13,
		Landfill = 14,
		Plopped = 15,
	};


	struct BuildingData
	{
		BuildingType buildingType;
		SC4BuildingPurposeType sc4BuildingType;
		uint32_t minCapacityLimit;
		bool makeLotHistorical;
	};

	bool ParseCheatString(const std::string_view& cheatString, BuildingData& data);
	BuildingWealth MapWealthType(BuildingType buildingType, uint32_t wealth);
	void ShowConvertedLotCount(int32_t count);

}