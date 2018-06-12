#pragma once
#include <Windows.h>
#include <vector>
#include <map>
bool runing = false;

using namespace std;


std::vector<std::string> playerGNameVec = { "PlayerMale", "PlayerFemale" };

std::vector<std::string> vehicleGNameVec = { "Uaz", "Buggy", "Dacia", "ABP_Motorbike", "BP_Motorbike", "Boat_PG117", "MiniBus" };

std::map<std::string, std::string> dropGNameMap = { { "Item_Head_G_01_Lv3_C", "Helm3" },{ "Item_Head_G_01_Lv3_", "Helm3" },
													{ "Item_Head_F_01_Lv2", "Helm2" },{ "Item_Head_F_01_Lv2_C", "Helm2" },{ "Item_Head_F_02_Lv2", "Helm2" },{ "Item_Head_F_02_Lv2_C", "Helm2" },
													{ "Item_Armor_C_01_Lv3", "Vest3" },{ "Item_Armor_C_01_Lv3_C", "Vest3" },{ "Item_Equip_Armor_Lv3_C", "Vest3" },{ "Item_Equip_Armor_Lv3", "Vest3" },
													{ "Item_Armor_D_01_Lv2", "Vest2" },{ "Item_Armor_D_01_Lv2_C", "Vest2" },{ "Item_Equip_Armor_Lv2_C", "Vest2" },{ "Item_Equip_Armor_Lv2", "Vest2" },
													{ "Item_Attach_Weapon_Muzzle_Suppressor_SniperRifle", "Supp(SR)" },{ "Item_Attach_Weapon_Muzzle_Suppressor_SniperRifle_C", "Supp(SR)" },
													{ "Item_Attach_Weapon_Muzzle_Compensator_SniperRifle", "Compen(SR)" },{ "Item_Attach_Weapon_Muzzle_Compensator_SniperRifle_C", "Compen(SR)" },
													{ "Item_Attach_Weapon_Muzzle_Suppressor_Large", "Supp(AR)" },{ "Item_Attach_Weapon_Muzzle_Suppressor_Large_C", "Supp(AR)" },
													{ "Item_Attach_Weapon_Muzzle_Compensator_Large", "Compen(AR)" },{ "Item_Attach_Weapon_Muzzle_Compensator_Large_C", "Compen(AR)" },
													//{ "Item_Attach_Weapon_Magazine_ExtendedQuickDraw_Large", "Ext_Quick(AR)" },{ "Item_Attach_Weapon_Magazine_ExtendedQuickDraw_Large_C", "Ext_Quick(AR)" },
													//{ "Item_Attach_Weapon_Magazine_ExtendedQuickDraw_SniperRifle", "Ext_Quick(SR)" },{ "Item_Attach_Weapon_Magazine_ExtendedQuickDraw_SniperRifle_C", "Ext_Quick(SR)" },
													{ "Item_Boost_PainKiller", "PK" },{ "Item_Boost_PainKiller_C", "PK" },
													{ "Item_Heal_FirstAid_C", "1Aid" },{ "Item_Heal_FirstAid", "1Aid" },
													{ " Item_Heal_MedKit_C", "MedKit" },{ " Item_Heal_MedKit", "MedKit" },
													{ "Item_Ammo_556mm_C", "556" },{ "Item_Ammo_556mm", "556" },
													{ "Item_Back_C_01_Lv3", "Bpv3" },{ "Item_Back_C_01_Lv3_C", "Bpv3" },
													{ "Item_Back_C_02_Lv3", "Bpv3" },{ "Item_Back_C_02_Lv3_C", "Bpv3" },
													{ "DeathDropItemPackage", "HeZi" },{ "DeathDropItemPackage_C", "HeZi" },
													{ "Item_Boost_EnergyDrink", "Drink" },{ "Item_Boost_EnergyDrink_C", "Drink" },
													{ "Item_Boost_AdrenalineSyringe", "XIANSU" },{ "Item_Boost_AdrenalineSyringe_C", "XIANSU" },
													{ "ItemSpawner_Item_Ghillie_01", "Ghillie" },
													{ "Item_Weapon_Kar98k", "Kar98" },{ "Item_Weapon_Mini14", "MINI-14" },{ "Item_Weapon_M16A4", "M16" },{ "Item_Weapon_HK416", "HK414" },{ "Item_Weapon_SCAR-L", "SCAR" },{ "Item_Weapon_SKS", "SKS" },
													{ "Item_Weapon_AK47", "AK47" },{ "Item_Weapon_AWM", "AWM" },{ "Item_Weapon_Groza", "Groza" },{ "Item_Weapon_M24", "M24" },{ "Item_Weapon_M249", "Pineapple" },{ "Item_Weapon_Mk14", "MK14" },
													{ "Item_Weapon_Pan", "Pan" },{ "Item_Weapon_Mini14", "MINI-14" },{ "Item_Weapon_M16A4", "M16" },{ "Item_Weapon_HK416", "HK414" },{ "Item_Weapon_SCAR-L", "SCAR" },{ "Item_Weapon_SKS", "SKS" },
													{ "Item_Weapon_AUG", "AUG" },{ "Item_Weapon_DP28", "DP28" },
													{ "Item_Attach_Weapon_Upper_ACOG_01", "4X" },{ "Item_Attach_Weapon_Upper_CQBSS", "8X" },{ "Item_Attach_Weapon_Upper_CQBSS_C", "8X" },
													{ "Item_Attach_Weapon_Upper_PM2_01", "15X" },{ "Item_Attach_Weapon_Upper_PM2_01_C", "15X" } };

struct Position
{
	float X;
	float Y;
	float Z;
};

