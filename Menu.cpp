#include "Menu.h"
#include "Controls.h"
#include "Hooks.h" 
#include "Interfaces.h"
#include "CRC32.h"

#define WINDOW_WIDTH 655
#define WINDOW_HEIGHT 400









aristoispejawindow Menu::Window;

void KnifeApplyCallbk()
{
	static ConVar* Meme = Interfaces::CVar->FindVar("cl_fullupdate");
	Meme->nFlags &= ~FCVAR_CHEAT;
	Interfaces::Engine->ClientCmd_Unrestricted("cl_fullupdate");
}

void SaveLegitCallbk()
{

	switch (Menu::Window.MiscTab.lstPlayers.GetIndex())
	{
	case 0:
		GUI.SaveWindowState(&Menu::Window, "RetroLegit1.cfg");
		break;
	case 1:
		GUI.SaveWindowState(&Menu::Window, "RetroLegit2.cfg");
		break;
	case 2:
		GUI.SaveWindowState(&Menu::Window, "RetroLegit3.cfg");
		break;
	case 3:
		GUI.SaveWindowState(&Menu::Window, "RetroRage1.cfg");
		break;
	case 4:
		GUI.SaveWindowState(&Menu::Window, "RetroRage2.cfg");
		break;
	case 5:
		GUI.SaveWindowState(&Menu::Window, "RetroRage3.cfg");
		break;
	case 6:
		GUI.SaveWindowState(&Menu::Window, "RetroMMHvH1.cfg");
		break;
	case 7:
		GUI.SaveWindowState(&Menu::Window, "RetroMMHvH2.cfg");
		break;
	case 8:
		GUI.SaveWindowState(&Menu::Window, "RetroMMHvH3.cfg");
		break;
	}
}

void LoadLegitCallbk()
{
	static ConVar* Meme = Interfaces::CVar->FindVar("cl_fullupdate");
	Interfaces::Engine->ClientCmd_Unrestricted("cl_fullupdate");

	switch (Menu::Window.MiscTab.lstPlayers.GetIndex())
	{
	case 0:
		GUI.LoadWindowState(&Menu::Window, "RetroLegit1.cfg");

		break;
	case 1:
		GUI.LoadWindowState(&Menu::Window, "RetroLegit2.cfg");

		break;
	case 2:
		GUI.LoadWindowState(&Menu::Window, "RetroLegit3.cfg");

		break;
	case 3:
		GUI.LoadWindowState(&Menu::Window, "RetroRage1.cfg");

		break;
	case 4:
		GUI.LoadWindowState(&Menu::Window, "RetroRage2.cfg");

		break;
	case 5:
		GUI.LoadWindowState(&Menu::Window, "RetroRage3.cfg");

		break;
	case 6:
		GUI.LoadWindowState(&Menu::Window, "RetroMMHvH1.cfg");

		break;
	case 7:
		GUI.LoadWindowState(&Menu::Window, "RetroMMHvH2.cfg");

		break;
	case 8:
		GUI.LoadWindowState(&Menu::Window, "RetroMMHvH3.cfg");

		break;
	}
}


void UnLoadCallbk()
{
	DoUnload = true;
}

void aristoispejawindow::Setup()
{
	EnableTabs(true);
	SetPosition(350, 50);
	SetSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	SetTitle("$Argent$");

	RegisterTab(&RageBotTab);
	RegisterTab(&LegitBotTab);
	RegisterTab(&VisualsTab);
	//RegisterTab(&SettingsTab);
	RegisterTab(&MiscTab);
	RegisterTab(&Skins);
	RegisterTab(&ColorsTab);


	RECT Client = GetClientArea();
	Client.bottom -= 29;

	RageBotTab.Setup();
	LegitBotTab.Setup();
	VisualsTab.Setup();
	MiscTab.Setup();
	SettingsTab.Setup();
	ColorsTab.Setup();
	Skins.Setup();

#pragma region Bottom Buttons

#pragma endregion Setting up the settings buttons
}

void CRageBotTab::Setup()
{
	SetTitle("RAGE");



#pragma region Aimbot

	AimbotGroup.SetPosition(10, 16);
	AimbotGroup.SetText("Section 1");
	AimbotGroup.AddTab(CGroupTab("Aimbot", 1));
	AimbotGroup.AddTab(CGroupTab("Hit Chance", 2));
	AimbotGroup.AddTab(CGroupTab("Min Damage", 3));
	AimbotGroup.AddTab(CGroupTab("Target", 4));
	AimbotGroup.SetSize(300, 330);
	RegisterControl(&AimbotGroup);
	
	//tab 1
	AimbotEnable.SetFileId("aim_enable");
	AimbotGroup.PlaceLabledControl(1,"Enable", this, &AimbotEnable);

	AimbotAutoFire.SetFileId("aim_autofire");
	AimbotGroup.PlaceLabledControl(1,"Auto Fire", this, &AimbotAutoFire);

	AimbotFov.SetFileId("aim_fov");
	AimbotFov.SetBoundaries(0.f, 180.f);
	AimbotFov.SetValue(39.f);
	AimbotGroup.PlaceLabledControl(1,"FOV Range", this, &AimbotFov);

	AimbotSilentAim.SetFileId("aim_silent");
	AimbotGroup.PlaceLabledControl(1,"Silent Aim", this, &AimbotSilentAim);

	AutoRevolver.SetFileId("aim_autorevolver");
	AimbotGroup.PlaceLabledControl(1,"Auto Revolver", this, &AutoRevolver);



	//tab 4
	TargetSelection.SetFileId("tgt_selection");
	TargetSelection.AddItem("FOV");
	TargetSelection.AddItem("Far Away");
	TargetSelection.AddItem("Lethal");
	TargetSelection.AddItem("Threat");
	TargetSelection.AddItem("2 Shot");
	AimbotGroup.PlaceLabledControl(4,"Selection", this, &TargetSelection);

	Hitboxhead.SetFileId("aim_head");
	AimbotGroup.PlaceLabledControl(4, "HITBOX-> HEAD", this, &Hitboxhead);

	Hitboxneck.SetFileId("aim_neck");
	AimbotGroup.PlaceLabledControl(4, "HITBOX-> NECK", this, &Hitboxneck);

	Hitboxarms.SetFileId("aim_chest");
	AimbotGroup.PlaceLabledControl(4, "HITBOX-> ARMS", this, &Hitboxarms);

	Hitboxchest.SetFileId("aim_chest");
	AimbotGroup.PlaceLabledControl(4, "HITBOX-> CHEST", this, &Hitboxchest);

	Hitboxstomach.SetFileId("aim_stomcach");
	AimbotGroup.PlaceLabledControl(4, "HITBOX-> PELVIS", this, &Hitboxstomach);

	Hitboxlegs.SetFileId("aim_legs");
	AimbotGroup.PlaceLabledControl(4, "HITBOX-> LEGS", this, &Hitboxlegs);

	//TargetHitscan.SetFileId("tgt_hitscan");
	//TargetHitscan.AddItem("None");
	//TargetHitscan.AddItem("Low");
	//TargetHitscan.AddItem("Medium");
	//	TargetHitscan.AddItem("High");
	//	AimbotGroup.PlaceLabledControl(0,"Hitscan", this, &TargetHitscan);

	//tab2

	WeaponCheck.SetFileId("weapon_chcek");
	AimbotGroup.PlaceLabledControl(2, "Enable Hitchance", this, &WeaponCheck);

	scoutChance.SetFileId("scout_chance");
	scoutChance.SetBoundaries(0, 100);
	scoutChance.SetValue(0);
	AimbotGroup.PlaceLabledControl(2, "Hitchance Scout", this, &scoutChance);

	AWPChance.SetFileId("awp_chance");
	AWPChance.SetBoundaries(0, 100);
	AWPChance.SetValue(0);
	AimbotGroup.PlaceLabledControl(2, "Hitchance AWP", this, &AWPChance);

	AutoChance.SetFileId("auto_chance");
	AutoChance.SetBoundaries(0, 100);
	AutoChance.SetValue(0);
	AimbotGroup.PlaceLabledControl(2, "Hitchance Auto", this, &AutoChance);

	RifleChance.SetFileId("rifle_chance");
	RifleChance.SetBoundaries(0, 100);
	RifleChance.SetValue(0);
	AimbotGroup.PlaceLabledControl(2, "Hitchance Rifle", this, &RifleChance);

	MPChance.SetFileId("mp_chance");
	MPChance.SetBoundaries(0, 100);
	MPChance.SetValue(0);
	AimbotGroup.PlaceLabledControl(2, "Hitchance MPS", this, &MPChance);

	PistolChance.SetFileId("pistol_chance");
	PistolChance.SetBoundaries(0, 100);
	PistolChance.SetValue(0);
	AimbotGroup.PlaceLabledControl(2, "Hitchance Pistols", this, &PistolChance);

	R8Chance.SetFileId("r8_chance");
	R8Chance.SetBoundaries(0, 100);
	R8Chance.SetValue(0);
	AimbotGroup.PlaceLabledControl(2, "Hitchance R8", this, &R8Chance);

	//tab 3

	scoutmindmg.SetFileId("scout_mindmg");
	scoutmindmg.SetBoundaries(0, 100);
	scoutmindmg.SetValue(0);
	AimbotGroup.PlaceLabledControl(3, "Min Dmg Scout", this, &scoutmindmg);

	AWPmindmg.SetFileId("awp_mindmg");
	AWPmindmg.SetBoundaries(0, 100);
	AWPmindmg.SetValue(0);
	AimbotGroup.PlaceLabledControl(3, "Min Dmg AWP", this, &AWPmindmg);

	Automindmg.SetFileId("auto_mindmg");
	Automindmg.SetBoundaries(0, 100);
	Automindmg.SetValue(0);
	AimbotGroup.PlaceLabledControl(3, "Min Dmg Auto", this, &Automindmg);

	Riflemindmg.SetFileId("rifle_mindmg");
	Riflemindmg.SetBoundaries(0, 100);
	Riflemindmg.SetValue(0);
	AimbotGroup.PlaceLabledControl(3, "Min Dmg Rifle", this, &Riflemindmg);

	MPmindmg.SetFileId("mp_mindmg");
	MPmindmg.SetBoundaries(0, 100);
	MPmindmg.SetValue(0);
	AimbotGroup.PlaceLabledControl(3, "Min Dmg MPS", this, &MPmindmg);

	Pistolmindmg.SetFileId("pistol_mindmg");
	Pistolmindmg.SetBoundaries(0, 100);
	Pistolmindmg.SetValue(0);
	AimbotGroup.PlaceLabledControl(3, "Min Dmg Pistols", this, &Pistolmindmg);

	R8mindmg.SetFileId("r8_mindmg");
	R8mindmg.SetBoundaries(0, 100);
	R8mindmg.SetValue(0);
	AimbotGroup.PlaceLabledControl(3, "Min Dmg R8", this, &R8mindmg);













#pragma endregion Aimbot Controls Get Setup in here





#pragma region AntiAim
	AntiAimGroup.SetPosition(325, 16); //344
	AntiAimGroup.SetText("Anti-Aimbot");
	AntiAimGroup.SetSize(300, 330);
	AntiAimGroup.AddTab(CGroupTab("AntiAims", 1));
	AntiAimGroup.AddTab(CGroupTab("Settings", 2));
	AntiAimGroup.AddTab(CGroupTab("LBYBreaker", 3));
	AntiAimGroup.AddTab(CGroupTab("Extra", 4));
	RegisterControl(&AntiAimGroup);


	//tab4
	AimbotResolver.SetFileId("acc_resolver");
	AimbotResolver.AddItem("Off");
	AimbotResolver.AddItem("Resolver Spread");
	AimbotResolver.AddItem("Resolver Test");
	AntiAimGroup.PlaceLabledControl(4, "Resolver", this, &AimbotResolver);

	PitchAimbotResolver.SetFileId("acc_resolverpitch");
	AntiAimGroup.PlaceLabledControl(4, "Pitch Correction", this, &PitchAimbotResolver);

	EnginePrediction.SetFileId("acc_engineprediciton");
	AntiAimGroup.PlaceLabledControl(4, "Engine Prediction", this, &EnginePrediction);




	//tab3


	BreakLBY.SetFileId("aa_break");
	BreakLBY.AddItem("Off");
	BreakLBY.AddItem("Custom°");
	BreakLBY.AddItem("135°");
	BreakLBY.AddItem("Cooming Soon°");
	AntiAimGroup.PlaceLabledControl(3, "LBY Breaker Angle", this, &BreakLBY);

	Antilby.SetFileId("antilby");
	Antilby.SetBoundaries(0, 180);
	Antilby.SetValue(95);
	AntiAimGroup.PlaceLabledControl(3, "Custom LBY Break", this, &Antilby);


	//tab 2

	AccuracyRecoil.SetFileId("acc_norecoil");
	AntiAimGroup.PlaceLabledControl(2, "Remove Recoil", this, &AccuracyRecoil);

	AccuracyAutoWall.SetFileId("acc_awall");
	AntiAimGroup.PlaceLabledControl(2, "Autowall", this, &AccuracyAutoWall);

	AccuracyBacktracking.SetFileId("acc_bcktrk");
	AntiAimGroup.PlaceLabledControl(2, "Backtrack", this, &AccuracyBacktracking);

	TargetMultipoint.SetFileId("acc_multipoint");
	AntiAimGroup.PlaceLabledControl(2, "MultiPoint", this, &TargetMultipoint);

	TargetPointscale.SetFileId("pointscale");
	TargetPointscale.SetBoundaries(0, 10);
	AntiAimGroup.PlaceLabledControl(2, "Pointscale", this, &TargetPointscale);

	BaimIfUnderXHealth.SetFileId("baimifhp");
	BaimIfUnderXHealth.SetBoundaries(0, 100);
	BaimIfUnderXHealth.SetValue(0);
	AntiAimGroup.PlaceLabledControl(2, "Baim If Hp ->", this, &BaimIfUnderXHealth);

	//tab1 

	AntiAimEnable.SetFileId("aa_enable");
	AntiAimGroup.PlaceLabledControl(1, "Enable AA", this, &AntiAimEnable);

	AntiAimPitch.SetFileId("aa_x");
	AntiAimPitch.AddItem("Off");
	AntiAimPitch.AddItem("Emotion");
	AntiAimPitch.AddItem("Minimal");
	AntiAimPitch.AddItem("Dynamic");
	AntiAimPitch.AddItem("Jitter");
	AntiAimGroup.PlaceLabledControl(1, "Pitch", this, &AntiAimPitch);

	AntiAimYaw.SetFileId("aa_y");
	AntiAimYaw.AddItem("Off");
	AntiAimYaw.AddItem("Backwards");
	AntiAimYaw.AddItem("Jitter");
	AntiAimYaw.AddItem("Lowerbody");
	AntiAimYaw.AddItem("Dynamic Swap");
	AntiAimYaw.AddItem("Fast Spin");
	AntiAimGroup.PlaceLabledControl(1, "Real Yaw", this, &AntiAimYaw);

	FakeYaw.SetFileId("fakeaa");
	FakeYaw.AddItem("Off");
	FakeYaw.AddItem("180 Jitter");
	FakeYaw.AddItem("180 LBY");
	FakeYaw.AddItem("LBY Jitter");
	FakeYaw.AddItem("Free");
	FakeYaw.AddItem("Spin");
	AntiAimGroup.PlaceLabledControl(1, "Fake Yaw", this, &FakeYaw);

	InAir.SetFileId("inair");
	InAir.AddItem("Off");
	InAir.AddItem("Half Spin");
	InAir.AddItem("Backwards");
	AntiAimGroup.PlaceLabledControl(1, "In Air", this, &InAir);

	SWSwitchEnable.SetFileId("manualaa");
	AntiAimGroup.PlaceLabledControl(1, "SWSwitch", this, &SWSwitchEnable);

	SWSwitchKey.SetFileId("swswitchkey");
	SWSwitchKey.SetKey(88);
	AntiAimGroup.PlaceLabledControl(1, "", this, &SWSwitchKey);


	//BaimIfUnderXHealth.SetFileId("acc_BaimIfUnderXHealth");
	//BaimIfUnderXHealth.SetBoundaries(0, 100);
	//BaimIfUnderXHealth.SetValue(0);
	//AntiAimGroup.PlaceLabledControl(0,"Body-Aim ", this, &BaimIfUnderXHealth);



	/*MoveYaw.SetFileId("aa_y_moving");
	MoveYaw.AddItem("Off");
	MoveYaw.AddItem("Fast Spin");
	MoveYaw.AddItem("Slow Spin");
	MoveYaw.AddItem("Jitter");
	MoveYaw.AddItem("180 Jitter");
	MoveYaw.AddItem("Backwards");
	MoveYaw.AddItem("Backwards Jitter");
	MoveYaw.AddItem("Sideways Switch");
	MoveYaw.AddItem("Sideways Right");
	MoveYaw.AddItem("Sideways Left");
	MoveYaw.AddItem("LBY");
	MoveYaw.AddItem("LBY Jitter");
	MoveYaw.AddItem("LBY Sideways");
	MoveYaw.AddItem("LBY Spin");
	MoveYaw.AddItem("LBY Break");
	AntiAimGroup.PlaceLabledControl(0,"Moving Yaw", this, &MoveYaw);

	MoveYawFake.SetFileId("movingfakeaa");
	MoveYawFake.AddItem("Off");
	MoveYawFake.AddItem("Fast Spin");
	MoveYawFake.AddItem("Slow Spin");
	MoveYawFake.AddItem("Jitter");
	MoveYawFake.AddItem("180 Jitter");
	MoveYawFake.AddItem("Backwards");
	MoveYawFake.AddItem("Backwards Jitter");
	MoveYawFake.AddItem("Sideways Switch");
	MoveYawFake.AddItem("Sideways Right");
	MoveYawFake.AddItem("Sideways Left");
	MoveYawFake.AddItem("LBY");
	MoveYawFake.AddItem("LBY Jitter");
	MoveYawFake.AddItem("LBY Sideways");
	MoveYawFake.AddItem("LBY Spin");
	MoveYawFake.AddItem("LBY Break");
	AntiAimGroup.PlaceLabledControl(0,"Moving Yaw Fake", this, &MoveYawFake);*/


#pragma endregion  AntiAim controls get setup in here
}

void CLegitBotTab::Setup()
{
	SetTitle("LEGIT");



#pragma region Aimbot
	AimbotGroup.SetPosition(10, 16);
	AimbotGroup.SetText("Aimbot");
	AimbotGroup.AddTab(CGroupTab("s", 1));
	AimbotGroup.AddTab(CGroupTab("u", 2));
	AimbotGroup.AddTab(CGroupTab("v", 3));
	AimbotGroup.AddTab(CGroupTab("W", 4));
	AimbotGroup.AddTab(CGroupTab("A", 5));
	AimbotGroup.AddTab(CGroupTab("Z", 6));
	AimbotGroup.AddTab(CGroupTab("M", 7));
	AimbotGroup.AddTab(CGroupTab("b", 8));
	AimbotGroup.AddTab(CGroupTab("f", 9));
	AimbotGroup.SetSize(600, 330);
	RegisterControl(&AimbotGroup);
	//tab 1
	AimbotEnable.SetFileId("aim_enable");
	AimbotGroup.PlaceLabledControl2(1,"Enable", this, &AimbotEnable);

	AimbotAutoFire.SetFileId("aim_autofire");
	AimbotGroup.PlaceLabledControl2(1,"Auto Fire", this, &AimbotAutoFire);

	AimbotFriendlyFire.SetFileId("aim_friendfire");
	AimbotGroup.PlaceLabledControl2(1,"Friendly Fire", this, &AimbotFriendlyFire);

	AimbotSmokeCheck.SetFileId("otr_smokecheck");
	AimbotGroup.PlaceLabledControl2(1,"Smoke Check", this, &AimbotSmokeCheck);

	AimbotKeyPress.SetFileId("aim_usekey");
	AimbotGroup.PlaceLabledControl2(1,"On Key", this, &AimbotKeyPress);

	AimbotKeyBind.SetFileId("aim_key");
	AimbotGroup.PlaceLabledControl2(1,"", this, &AimbotKeyBind);

	AimbotBacktrack.SetFileId("legit_backtrack");
	AimbotGroup.PlaceLabledControl2(1, "Backtrack Ticks", this, &AimbotBacktrack);

	TickModulation.SetFileId("tick_modulate");
	TickModulation.SetBoundaries(0.1f, 13.f);
	TickModulation.SetValue(13.f);
	AimbotGroup.PlaceLabledControl2(1, "Tick Modulation", this, &TickModulation);

	legitaa.SetFileId("legit_aa");
	legitaa.AddItem("None");
	legitaa.AddItem("Backwards");
	legitaa.AddItem("Switch");
	AimbotGroup.PlaceLabledControl2(1, "Legit AA", this, &legitaa);

	legitaaswitch.SetFileId("legitaaswitch");
	AimbotGroup.PlaceLabledControl2(1, "", this, &legitaaswitch);

	Legitaareolver.SetFileId("legitaaresolver");
	AimbotGroup.PlaceLabledControl2(1, "LegitAA Resolver", this, &Legitaareolver);

	//tab2

	TriggerEnable.SetFileId("trig_enable");
	AimbotGroup.PlaceLabledControl2(2, "Enable", this, &TriggerEnable);

	TriggerDelay.SetFileId("trig_time");
	TriggerDelay.SetBoundaries(0, 100);
	TriggerDelay.SetValue(1);
	AimbotGroup.PlaceLabledControl2(2, "Delay", this, &TriggerDelay);

	TriggerRecoil.SetFileId("trig_recoil");
	AimbotGroup.PlaceLabledControl2(2, "Recoil Control", this, &TriggerRecoil);

	TriggerSmokeCheck.SetFileId("trig_smokecheck");
	AimbotGroup.PlaceLabledControl2(2, "Smoke Check", this, &TriggerSmokeCheck);

	TriggerKeyPress.SetFileId("trig_onkey");
	AimbotGroup.PlaceLabledControl2(2, "On Key Press", this, &TriggerKeyPress);

	TriggerKeyBind.SetFileId("trig_key");
	AimbotGroup.PlaceLabledControl2(2, "", this, &TriggerKeyBind);


	//tab 3
	TriggerHead.SetFileId("trig_head");
	AimbotGroup.PlaceLabledControl2(3, "Head", this, &TriggerHead);

	TriggerChest.SetFileId("trig_chest");
	AimbotGroup.PlaceLabledControl2(3, "Chest", this, &TriggerChest);

	TriggerStomach.SetFileId("trig_stomach");
	AimbotGroup.PlaceLabledControl2(3, "Stomach", this, &TriggerStomach);

	TriggerArms.SetFileId("trig_arms");
	AimbotGroup.PlaceLabledControl2(3, "Arms", this, &TriggerArms);

	TriggerLegs.SetFileId("trig_legs");
	AimbotGroup.PlaceLabledControl2(3, "Legs", this, &TriggerLegs);

	TriggerTeammates.SetFileId("trig_teammates");
	AimbotGroup.PlaceLabledControl2(3, "Friendly Fire", this, &TriggerTeammates);

	//tab 4


	WeaponMainSpeed.SetFileId("main_speed");
	WeaponMainSpeed.SetBoundaries(0.f, 100.f);
	WeaponMainSpeed.SetValue(1.f);
	AimbotGroup.PlaceLabledControl2(4, "Aim Speed", this, &WeaponMainSpeed);

	WeaponMainFoV.SetFileId("main_fov");
	WeaponMainFoV.SetBoundaries(0.f, 30.f);
	WeaponMainFoV.SetValue(3.5f);
	AimbotGroup.PlaceLabledControl2(4, "Field of View", this, &WeaponMainFoV);

	WeaponMainRecoil.SetFileId("main_recoil");
	WeaponMainRecoil.SetBoundaries(0.f, 2.f);
	WeaponMainRecoil.SetValue(0.f);
	AimbotGroup.PlaceLabledControl2(4, "Recoil Control", this, &WeaponMainRecoil);
	WeaponMainHitbox.SetFileId("main_hitbox");
	WeaponMainHitbox.AddItem("Head");
	WeaponMainHitbox.AddItem("Neck");
	WeaponMainHitbox.AddItem("Chest");
	WeaponMainHitbox.AddItem("Stomach");
	WeaponMainHitbox.AddItem("Nearest");
	AimbotGroup.PlaceLabledControl2(4, "Hitbox", this, &WeaponMainHitbox);

	//tab 5
	WeaponPistSpeed.SetFileId("pist_speed");
	WeaponPistSpeed.SetBoundaries(0.f, 100.f);
	WeaponPistSpeed.SetValue(1.0f);
	AimbotGroup.PlaceLabledControl2(5, "Aim Speed", this, &WeaponPistSpeed);

	WeaponPistFoV.SetFileId("pist_fov");
	WeaponPistFoV.SetBoundaries(0.f, 30.f);
	WeaponPistFoV.SetValue(3.f);
	AimbotGroup.PlaceLabledControl2(5, "Field of View", this, &WeaponPistFoV);

	WeaponPistRecoil.SetFileId("pist_recoil");
	WeaponPistRecoil.SetBoundaries(0.f, 2.f);
	WeaponPistRecoil.SetValue(0.f);
	AimbotGroup.PlaceLabledControl2(5, "Recoil Control", this, &WeaponPistRecoil);

	WeaponPistHitbox.SetFileId("pist_hitbox");
	WeaponPistHitbox.AddItem("Head");
	WeaponPistHitbox.AddItem("Neck");
	WeaponPistHitbox.AddItem("Chest");
	WeaponPistHitbox.AddItem("Stomach");
	WeaponPistHitbox.AddItem("Nearest");
	AimbotGroup.PlaceLabledControl2(5, "Hitbox", this, &WeaponPistHitbox);

	//tab6
	WeaponSnipSpeed.SetFileId("snip_speed");
	WeaponSnipSpeed.SetBoundaries(0.f, 100.f);
	WeaponSnipSpeed.SetValue(1.5f);
	AimbotGroup.PlaceLabledControl2(6, "Aim Speed", this, &WeaponSnipSpeed);

	WeaponSnipFoV.SetFileId("snip_fov");
	WeaponSnipFoV.SetBoundaries(0.f, 30.f);
	WeaponSnipFoV.SetValue(2.f);
	AimbotGroup.PlaceLabledControl2(6, "Field of View", this, &WeaponSnipFoV);

	WeaponSnipRecoil.SetFileId("snip_recoil");
	WeaponSnipRecoil.SetBoundaries(0.f, 2.f);
	WeaponSnipRecoil.SetValue(0.f);
	AimbotGroup.PlaceLabledControl2(6, "Recoil Control", this, &WeaponSnipRecoil);

	WeaponSnipHitbox.SetFileId("snip_hitbox");
	WeaponSnipHitbox.AddItem("Head");
	WeaponSnipHitbox.AddItem("Neck");
	WeaponSnipHitbox.AddItem("Chest");
	WeaponSnipHitbox.AddItem("Stomach");
	WeaponSnipHitbox.AddItem("Nearest");
	AimbotGroup.PlaceLabledControl2(6, "Hitbox", this, &WeaponSnipHitbox);

	//tab7
	WeaponMpSpeed.SetFileId("mps_speed");
	WeaponMpSpeed.SetBoundaries(0.f, 100.f);
	WeaponMpSpeed.SetValue(1.0f);
	AimbotGroup.PlaceLabledControl2(7, "Aim Speed", this, &WeaponMpSpeed);

	WeaponMpFoV.SetFileId("mps_fov");
	WeaponMpFoV.SetBoundaries(0.f, 30.f);
	WeaponMpFoV.SetValue(4.f);
	AimbotGroup.PlaceLabledControl2(7, "Field of View", this, &WeaponMpFoV);

	WeaponMpRecoil.SetFileId("mps_recoil");
	WeaponMpRecoil.SetBoundaries(0.f, 2.f);
	WeaponMpRecoil.SetValue(0.f);
	AimbotGroup.PlaceLabledControl2(7, "Recoil Control", this, &WeaponMpRecoil);

	WeaponMpHitbox.SetFileId("mps_hitbox");
	WeaponMpHitbox.AddItem("Head");
	WeaponMpHitbox.AddItem("Neck");
	WeaponMpHitbox.AddItem("Chest");
	WeaponMpHitbox.AddItem("Stomach");
	WeaponMpHitbox.AddItem("Nearest");
	AimbotGroup.PlaceLabledControl2(7, "Hitbox", this, &WeaponMpHitbox);

	//tab8

	WeaponShotgunSpeed.SetFileId("shotgun_speed");
	WeaponShotgunSpeed.SetBoundaries(0.f, 100.f);
	WeaponShotgunSpeed.SetValue(1.0f);
	AimbotGroup.PlaceLabledControl2(8, "Aim Speed", this, &WeaponShotgunSpeed);

	WeaponShotgunFoV.SetFileId("shotgun_fov");
	WeaponShotgunFoV.SetBoundaries(0.f, 30.f);
	WeaponShotgunFoV.SetValue(3.f);
	AimbotGroup.PlaceLabledControl2(8, "Field of View", this, &WeaponShotgunFoV);

	WeaponShotgunRecoil.SetFileId("snip_recoil");
	WeaponShotgunRecoil.SetBoundaries(0.f, 2.f);
	WeaponShotgunRecoil.SetValue(0.f);
	AimbotGroup.PlaceLabledControl2(8, "Recoil Control", this, &WeaponShotgunRecoil);

	WeaponShotgunHitbox.SetFileId("shotgun_hitbox");
	WeaponShotgunHitbox.AddItem("Head");
	WeaponShotgunHitbox.AddItem("Neck");
	WeaponShotgunHitbox.AddItem("Chest");
	WeaponShotgunHitbox.AddItem("Stomach");
	WeaponShotgunHitbox.AddItem("Nearest");
	AimbotGroup.PlaceLabledControl2(8, "Hitbox", this, &WeaponShotgunHitbox);

	//tab9

	WeaponMGSpeed.SetFileId("mg_speed");
	WeaponMGSpeed.SetBoundaries(0.f, 100.f);
	WeaponMGSpeed.SetValue(1.0f);
	AimbotGroup.PlaceLabledControl2(9, "Aim Speed", this, &WeaponMGSpeed);

	WeaponMGFoV.SetFileId("mg_fov");
	WeaponMGFoV.SetBoundaries(0.f, 30.f);
	WeaponMGFoV.SetValue(4.f);
	AimbotGroup.PlaceLabledControl2(9, "Field of View", this, &WeaponMGFoV);

	WeaponMGRecoil.SetFileId("mg_recoil");
	WeaponMGRecoil.SetBoundaries(0.f, 2.f);
	WeaponMGRecoil.SetValue(0.f);
	AimbotGroup.PlaceLabledControl2(9, "Recoil Control", this, &WeaponMGRecoil);


	WeaponMGHitbox.SetFileId("mg_hitbox");
	WeaponMGHitbox.AddItem("Head");
	WeaponMGHitbox.AddItem("Neck");
	WeaponMGHitbox.AddItem("Chest");
	WeaponMGHitbox.AddItem("Stomach");
	WeaponMGHitbox.AddItem("Nearest");
	AimbotGroup.PlaceLabledControl2(9, "Hitbox", this, &WeaponMGHitbox);


#pragma endregion Aimbot shit










#pragma endregion
}

void CVisualTab::Setup()
{
	SetTitle("VISUALS");


	OptionsGroup.SetText("Options");
	OptionsGroup.SetPosition(10, 16);
	OptionsGroup.AddTab(CGroupTab("ESP", 1));
	OptionsGroup.AddTab(CGroupTab("Filter", 2));
	OptionsGroup.AddTab(CGroupTab("Extra", 3));
	OptionsGroup.SetSize(600, 330);
	RegisterControl(&OptionsGroup);
	//tab1
	OptionsBox.SetFileId("otr_showbox");
	OptionsBox.AddItem("off");
	OptionsBox.AddItem("Custom");
	OptionsBox.AddItem("Normal");
	OptionsGroup.PlaceLabledControl(1,"Show Box", this, &OptionsBox);

	Chams.SetFileId("esp_chams");
	Chams.AddItem("None");
	Chams.AddItem("3D");
	Chams.AddItem("2D");
	OptionsGroup.PlaceLabledControl(1, "Chams", this, &Chams);

	OptionsGlow.SetFileId("otp_glow");
	OptionsGroup.PlaceLabledControl(1, "Glow", this, &OptionsGlow);

	OptionsName.SetFileId("opt_name");
	OptionsGroup.PlaceLabledControl(1,"Name", this, &OptionsName);

	OptionsWeapon.SetFileId("opt_weapon");
	OptionsGroup.PlaceLabledControl(1,"Weapon", this, &OptionsWeapon);

	OptionHealthEnable.SetFileId("opt_health");
	OptionsGroup.PlaceLabledControl(1,"Health", this, &OptionHealthEnable);

	OptionsArmor.SetFileId("opt_armor");
	OptionsGroup.PlaceLabledControl(1,"Armor", this, &OptionsArmor);

	OptionsSkeleton.SetFileId("opt_bone");
	OptionsGroup.PlaceLabledControl(1,"Skeleton", this, &OptionsSkeleton);




	//tab2

	FiltersEnemiesOnly.SetFileId("ftr_enemyonly");
	OptionsGroup.PlaceLabledControl(2, "Enemies Only", this, &FiltersEnemiesOnly);

	FiltersC4.SetFileId("bomb");
	OptionsGroup.PlaceLabledControl(2, "C4", this, &FiltersC4);

	FiltersWeapons.SetFileId("ftr_weaponss");
	OptionsGroup.PlaceLabledControl(2, "Weapons", this, &FiltersWeapons);

	OptionsInfo.SetFileId("opt_info");
	OptionsGroup.PlaceLabledControl(2, "Info", this, &OptionsInfo);

	OtherNoSmoke.SetFileId("eff_nosmoke");
	OptionsGroup.PlaceLabledControl(2, "No Smoke", this, &OtherNoSmoke);

	OtherNoScope.SetFileId("eff_noscope");
	OptionsGroup.PlaceLabledControl(2, "No Scope", this, &OtherNoScope);

	OtherNoVisualRecoil.SetFileId("eff_visrecoil");
	OptionsGroup.PlaceLabledControl(2, "No Visual Recoil", this, &OtherNoVisualRecoil);

	OtherNoFlash.SetFileId("otr_noflash");
	OptionsGroup.PlaceLabledControl(2, "No Flash ", this, &OtherNoFlash);

	AALines.SetFileId("opt_aalines");
	OptionsGroup.PlaceLabledControl(2, "AA lines", this, &AALines);


	ManualaaIndicator.SetFileId("opt_aaindicaor");
	OptionsGroup.PlaceLabledControl(2, "AA Indicator", this, &ManualaaIndicator);

	LBYIdicador.SetFileId("opt_indicaor");
	OptionsGroup.PlaceLabledControl(2, "LBY Indicator", this, &LBYIdicador);

	OptionsCompRank.SetFileId("opt_comprank");
	OptionsGroup.PlaceLabledControl(2, "Ranks", this, &OptionsCompRank);






	//tab3


	BulletTracers.SetFileId("bullettracers");
	OptionsGroup.PlaceLabledControl(3, "Bullet Tracers", this, &BulletTracers);

	DamageIndicator.SetFileId("damageindicator");
	OptionsGroup.PlaceLabledControl(3, "Show Damage", this, &DamageIndicator);

	SpreadCrosshair.SetFileId("spreadcrosshair");
	OptionsGroup.PlaceLabledControl(3, "Spread Crosshair", this, &SpreadCrosshair);

	OtherHitmarker.SetFileId("otr_hitmarker");
	OptionsGroup.PlaceLabledControl(3, "Hitmarker", this, &OtherHitmarker);

	HitmarkerSound.SetFileId("otr_hitmarkersound");
	OptionsGroup.PlaceLabledControl(3, "Hitmarker Sound", this, &HitmarkerSound);

	Normalcrosshair.SetFileId("otr_crosshair");
	OptionsGroup.PlaceLabledControl(3, "AWP Crosshair", this, &Normalcrosshair);

	SpecList.SetFileId("otr_speclist");
	OptionsGroup.PlaceLabledControl(3, "Show Spectators", this, &SpecList);

	OtherRadar.SetFileId("otr_radar");
	OptionsGroup.PlaceLabledControl(3, "Show Radar", this, &OtherRadar);

	OtherNoHands.SetFileId("otr_hands");
	OtherNoHands.AddItem("Off");
	OtherNoHands.AddItem("Glass");
	OtherNoHands.AddItem("Transparent");
	OtherNoHands.AddItem("Wireframe");
	OptionsGroup.PlaceLabledControl(3, "Hands", this, &OtherNoHands);

	NightMode.SetFileId("nightmode");
	OptionsGroup.PlaceLabledControl(3, "Night Mode", this, &NightMode);


	OtherViewmodelFOV.SetFileId("otr_viewfov");
	OtherViewmodelFOV.SetBoundaries(0.f, 180.f);
	OtherViewmodelFOV.SetValue(0.f);
	OptionsGroup.PlaceLabledControl(3, "Viewmodel FOV", this, &OtherViewmodelFOV);

	OtherFOV.SetFileId("otr_fov");
	OtherFOV.SetBoundaries(0.f, 180.f);
	OtherFOV.SetValue(0.f);
	OptionsGroup.PlaceLabledControl(3, "Field of View", this, &OtherFOV);




	FiltersPlayers.SetState(true);








	FiltersC4.SetState(true);





#pragma endregion Setting up the Options controls




}

void CMiscTab::Setup()
{
	SetTitle("MISC");

	/*SaveButton3.SetText("Save");
	SaveButton3.SetCallback(SaveLegitCallbk);
	SaveButton3.SetPosition(500, 16);
	SaveButton3.SetSize(70, 70);
	RegisterControl(&SaveButton3);

	LoadButton3.SetText("Load");
	LoadButton3.SetCallback(LoadLegitCallbk);
	LoadButton3.SetPosition(500, 80);
	LoadButton3.SetSize(70, 70);
	RegisterControl(&LoadButton3);*/

#pragma region Other
	OtherGroup.SetPosition(10, 16);
	OtherGroup.SetSize(600, 330);
	OtherGroup.AddTab(CGroupTab("Misc", 1));
	OtherGroup.AddTab(CGroupTab("Filter", 2));
	OtherGroup.AddTab(CGroupTab("Config", 3));
	OtherGroup.SetText("Other");
	RegisterControl(&OtherGroup);
	//tab 1
	OtherSafeMode.SetFileId("otr_safemode");
	OtherSafeMode.SetState(true);
	OtherGroup.PlaceLabledControl(1,"Anti Untrusted", this, &OtherSafeMode);


	OtherCircle.SetFileId("otr_criclebutton");
	OtherGroup.PlaceLabledControl(1, "Circle Strafe", this, &OtherCircle);


	OtherClantag.SetFileId("otr_clantag");
	OtherClantag.AddItem("Off");
	OtherClantag.AddItem("Static Retronix.club");
	OtherClantag.AddItem("Moving Retronix.club");
	OtherClantag.AddItem("Custom Kiduahook");
	OtherClantag.AddItem("Sippin' Skeet");
	OtherGroup.PlaceLabledControl(1, "ClanTag", this, &OtherClantag);


	//tab 2

	AutoPistol.SetFileId("otr_autopistol");
	OtherGroup.PlaceLabledControl(2, "Auto Pistol", this, &AutoPistol);

	FakeAngleChams.SetFileId("aa_fkechams");
	OtherGroup.PlaceLabledControl(2, "Fake Angles Chams", this, &FakeAngleChams);

	OtherThirdperson.SetFileId("aa_thirdpsr");
	OtherGroup.PlaceLabledControl(2, "Thirdperson", this, &OtherThirdperson);

	ThirdPersonKeyBind.SetFileId("aa_thirdpersonKey");
	OtherGroup.PlaceLabledControl(2,"", this, &ThirdPersonKeyBind);

	OtherThirdpersonRange.SetFileId("thirdpersonrange");
	OtherThirdpersonRange.SetBoundaries(1, 1000);
	OtherThirdpersonRange.SetValue(180);
	OtherGroup.PlaceLabledControl(2, "Thirdp Range", this, &OtherThirdpersonRange);

	SeeTpangle.SetFileId("aa_thirdpersonMode");
	SeeTpangle.AddItem("Real");
	SeeTpangle.AddItem("Fake");
	SeeTpangle.AddItem("LBY");
	OtherGroup.PlaceLabledControl(2, "Thirdperson Angle", this, &SeeTpangle);

	OtherAutoJump.SetFileId("otr_autojump");
	OtherGroup.PlaceLabledControl(2,"Auto Jump", this, &OtherAutoJump);


	OtherAutoStrafe.SetFileId("otr_strafe");
	OtherAutoStrafe.AddItem("Off");
	OtherAutoStrafe.AddItem("Legit");
	OtherAutoStrafe.AddItem("Rage");
	OtherGroup.PlaceLabledControl(2,"Auto Strafer", this, &OtherAutoStrafe);


	//customclan.SetFileId("otr_customclan");
	//customclan.SetText("$Argent$");
	//OtherGroup.PlaceLabledControl(0,"Custom Clantag", this, &customclan);


	//tab 3

	lstPlayers.SetFileId("configs");
	lstPlayers.AddItem("Legit 1");
	lstPlayers.AddItem("Legit 2");
	lstPlayers.AddItem("Legit 3");
	lstPlayers.AddItem("Rage 1");
	lstPlayers.AddItem("Rage 2");
	lstPlayers.AddItem("Rage 3");
	lstPlayers.AddItem("MM HvH 1");
	lstPlayers.AddItem("MM HvH 2");
	lstPlayers.AddItem("MM HvH 3");
	OtherGroup.PlaceLabledControl(3,"Configs", this, &lstPlayers);

	SaveButton3.SetFileId("save");
	SaveButton3.SetCallback(SaveLegitCallbk);
	OtherGroup.PlaceLabledControl(3, "", this, &SaveButton3);

	LoadButton3.SetFileId("load");
	LoadButton3.SetCallback(LoadLegitCallbk);
	OtherGroup.PlaceLabledControl(3, "", this, &LoadButton3);



#pragma endregion other random options


}

void CColorTab::Setup()
{
	SetTitle("COLORS");
#pragma region Visual Colors


	Colors.SetPosition(16, 16);
	Colors.SetText("Colors");
	Colors.AddTab(CGroupTab("CT Box", 1));
	Colors.AddTab(CGroupTab("T Box", 2));
	Colors.AddTab(CGroupTab("CT Chams", 3));
	Colors.AddTab(CGroupTab("Glow", 4));
	Colors.AddTab(CGroupTab("Bullettracers", 5));
	Colors.SetSize(600, 330);
	RegisterControl(&Colors);
	//tab1
	
	CTColorVisR.SetFileId("ct_color_vis_r");
	CTColorVisR.SetBoundaries(0.f, 255.f);
	CTColorVisR.SetValue(0.f);
	Colors.PlaceLabledControl(1, "Visible_Red", this, &CTColorVisR);

	CTColorVisG.SetFileId("ct_color_vis_g");
	CTColorVisG.SetBoundaries(0.f, 255.f);
	CTColorVisG.SetValue(230.f);
	Colors.PlaceLabledControl(1, "Visible_Green", this, &CTColorVisG);

	CTColorVisB.SetFileId("ct_color_vis_b");
	CTColorVisB.SetBoundaries(0.f, 255.f);
	CTColorVisB.SetValue(15.f);
	Colors.PlaceLabledControl(1, "Visible_Blue", this, &CTColorVisB);


	CTColorNoVisR.SetFileId("ct_color_no_vis_r");
	CTColorNoVisR.SetBoundaries(0.f, 255.f);
	CTColorNoVisR.SetValue(0.f);
	Colors.PlaceLabledControl(1, "NotVisible_Red", this, &CTColorNoVisR);

	CTColorNoVisG.SetFileId("ct_color_no_vis_g");
	CTColorNoVisG.SetBoundaries(0.f, 255.f);
	CTColorNoVisG.SetValue(235.f);
	Colors.PlaceLabledControl(1, "NotVisible_Green", this, &CTColorNoVisG);

	CTColorNoVisB.SetFileId("ct_color_no_vis_b");
	CTColorNoVisB.SetBoundaries(0.f, 255.f);
	CTColorNoVisB.SetValue(10.f);
	Colors.PlaceLabledControl(1, "NotVisible_Blue", this, &CTColorNoVisB);



	//tab2




	TColorVisR.SetFileId("t_color_vis_r");
	TColorVisR.SetBoundaries(0.f, 255.f);
	TColorVisR.SetValue(0.f);
	Colors.PlaceLabledControl(2, "Visible_Red", this, &TColorVisR);

	TColorVisG.SetFileId("t_color_vis_g");
	TColorVisG.SetBoundaries(0.f, 255.f);
	TColorVisG.SetValue(50.f);
	Colors.PlaceLabledControl(2, "Visible_Green", this, &TColorVisG);

	TColorVisB.SetFileId("t_color_vis_b");
	TColorVisB.SetBoundaries(0.f, 255.f);
	TColorVisB.SetValue(220.f);
	Colors.PlaceLabledControl(2, "Visible_Blue", this, &TColorVisB);

	TColorNoVisR.SetFileId("t_color_no_vis_r");
	TColorNoVisR.SetBoundaries(0.f, 255.f);
	TColorNoVisR.SetValue(0.f);
	Colors.PlaceLabledControl(2,"NotVisible_Red", this, &TColorNoVisR);

	TColorNoVisG.SetFileId("t_color_no_vis_g");
	TColorNoVisG.SetBoundaries(0.f, 255.f);
	TColorNoVisG.SetValue(50.f);
	Colors.PlaceLabledControl(2,"NotVisible_Green", this, &TColorNoVisG);

	TColorNoVisB.SetFileId("t_color_no_vis_b");
	TColorNoVisB.SetBoundaries(0.f, 255.f);
	TColorNoVisB.SetValue(220.f);
	Colors.PlaceLabledControl(2,"NotVisible_Blue", this, &TColorNoVisB);


	//tab3

	VisR.SetFileId("visr");
	VisR.SetBoundaries(0.f, 255.f);
	VisR.SetValue(40.f);
	Colors.PlaceLabledControl(3, "Vis Red", this, &VisR);

	VisG.SetFileId("visg");
	VisG.SetBoundaries(0.f, 255.f);
	VisG.SetValue(120.f);
	Colors.PlaceLabledControl(3, "Vis Green", this, &VisG);

	VisB.SetFileId("visb");
	VisB.SetBoundaries(0.f, 255.f);
	VisB.SetValue(170.f);
	Colors.PlaceLabledControl(3, "Vis Blue", this, &VisB);
	
	//tab4 

	NotVisR.SetFileId("notvisr");
	NotVisR.SetBoundaries(0.f, 255.f);
	NotVisR.SetValue(210.f);
	Colors.PlaceLabledControl(3, "Not Vis Red", this, &NotVisR);

	NotVisG.SetFileId("notvisg");
	NotVisG.SetBoundaries(0.f, 255.f);
	NotVisG.SetValue(20.f);
	Colors.PlaceLabledControl(3, "Not Vis Green", this, &NotVisG);

	NotVisB.SetFileId("notvisrb");
	NotVisB.SetBoundaries(0.f, 255.f);
	NotVisB.SetValue(70.f);
	Colors.PlaceLabledControl(3, "Not Vis Blue", this, &NotVisB);





	GlowR.SetFileId("GlowR");
	GlowR.SetBoundaries(0.f, 255.f);
	GlowR.SetValue(230.f);
	Colors.PlaceLabledControl(4,"Red", this, &GlowR);

	GlowG.SetFileId("GlowG");
	GlowG.SetBoundaries(0.f, 255.f);
	GlowG.SetValue(0.f);
	Colors.PlaceLabledControl(4,"Green", this, &GlowG);

	GlowB.SetFileId("GlowB");
	GlowB.SetBoundaries(0.f, 255.f);
	GlowB.SetValue(0.f);
	Colors.PlaceLabledControl(4,"Blue", this, &GlowB);



	BULLETCT_R.SetFileId("bulletctR");
	BULLETCT_R.SetBoundaries(0.f, 255.f);
	BULLETCT_R.SetValue(0.f);
	Colors.PlaceLabledControl(5,"CT_Red", this, &BULLETCT_R);

	BULLETCT_G.SetFileId("bulletctG");
	BULLETCT_G.SetBoundaries(0.f, 255.f);
	BULLETCT_G.SetValue(0.f);
	Colors.PlaceLabledControl(5,"CT_Green", this, &BULLETCT_G);

	BULLETCT_B.SetFileId("bulletctB");
	BULLETCT_B.SetBoundaries(0.f, 255.f);
	BULLETCT_B.SetValue(0.f);
	Colors.PlaceLabledControl(5,"CT_Blue", this, &BULLETCT_B);

	BULLET_T_R.SetFileId("bullettR");
	BULLET_T_R.SetBoundaries(0.f, 255.f);
	BULLET_T_R.SetValue(230.f);
	Colors.PlaceLabledControl(5,"T_Red", this, &BULLET_T_R);

	BULLET_T_G.SetFileId("bullettG");
	BULLET_T_G.SetBoundaries(0.f, 255.f);
	BULLET_T_G.SetValue(0.f);
	Colors.PlaceLabledControl(5,"T_Green", this, &BULLET_T_G);


	BULLET_T_B.SetFileId("bullettB");
	BULLET_T_B.SetBoundaries(0.f, 255.f);
	BULLET_T_B.SetValue(0.f);
	Colors.PlaceLabledControl(5,"T_Blue", this, &BULLET_T_B);











#pragma endregion
}

void SkinsTab::Setup()
{
	SetTitle("SkinChanger");


	SkinActive.SetPosition(25, 16);
	SkinActive.SetText("Enable");
	RegisterControl(&SkinActive);

	SkinEnable.SetFileId("skin_enable");
	SkinEnable.SetPosition(10, 16);
	RegisterControl(&SkinEnable);

	SkinApply.SetText("Apply");
	SkinApply.SetCallback(KnifeApplyCallbk);
	SkinApply.SetPosition(500, 16);
	SkinApply.SetSize(70, 70);
	RegisterControl(&SkinApply);

#pragma region Knife
	KnifeGroup.SetPosition(10, 16);
	KnifeGroup.SetText("Skins");
	KnifeGroup.AddTab(CGroupTab("Settings", 1));
	KnifeGroup.AddTab(CGroupTab("Knifes", 2));
	KnifeGroup.AddTab(CGroupTab("Rifles", 3));
	KnifeGroup.AddTab(CGroupTab("Snipers", 4));
	KnifeGroup.AddTab(CGroupTab("Pistols", 5));
	KnifeGroup.AddTab(CGroupTab("Extra", 6));
	KnifeGroup.AddTab(CGroupTab("Heavys", 7));
	KnifeGroup.AddTab(CGroupTab("Shotguns", 8));
	KnifeGroup.SetSize(600, 330);
	RegisterControl(&KnifeGroup);

	//tab1
	SkinEnable.SetFileId("knife_model");
	KnifeGroup.PlaceLabledControl(1, "Enable", this, &SkinEnable);


	SkinApply.SetFileId("applyskin");
	SkinApply.SetCallback(KnifeApplyCallbk);
	KnifeGroup.PlaceLabledControl(1, "", this, &SkinApply);


	//tab2
	KnifeModel.SetFileId("knife_model");
	KnifeModel.AddItem("Bayonet");
	KnifeModel.AddItem("Bowie Knife");
	KnifeModel.AddItem("Butterfly Knife");
	KnifeModel.AddItem("Falchion Knife");
	KnifeModel.AddItem("Flip Knife");
	KnifeModel.AddItem("Gut Knife");
	KnifeModel.AddItem("Huntsman Knife");
	KnifeModel.AddItem("Karambit");
	KnifeModel.AddItem("M9 Bayonet");
	KnifeModel.AddItem("Shadow Daggers");
	KnifeGroup.PlaceLabledControl(2,"Knife", this, &KnifeModel);

	KnifeSkin.SetFileId("knife_skin");
	KnifeSkin.AddItem("None");
	KnifeSkin.AddItem("Crimson Web");
	KnifeSkin.AddItem("Bone Mask");
	KnifeSkin.AddItem("Fade");
	KnifeSkin.AddItem("Night");
	KnifeSkin.AddItem("Blue Steel");
	KnifeSkin.AddItem("Stained");
	KnifeSkin.AddItem("Case Hardened");
	KnifeSkin.AddItem("Slaughter");
	KnifeSkin.AddItem("Safari Mesh");
	KnifeSkin.AddItem("Boreal Forest");
	KnifeSkin.AddItem("Ultraviolet");
	KnifeSkin.AddItem("Urban Masked");
	KnifeSkin.AddItem("Scorched");
	KnifeSkin.AddItem("Rust Coat");
	KnifeSkin.AddItem("Tiger Tooth");
	KnifeSkin.AddItem("Damascus Steel");
	KnifeSkin.AddItem("Damascus Steel");
	KnifeSkin.AddItem("Marble Fade");
	KnifeSkin.AddItem("Rust Coat");
	KnifeSkin.AddItem("Doppler Ruby");
	KnifeSkin.AddItem("Doppler Sapphire");
	KnifeSkin.AddItem("Doppler Blackpearl");
	KnifeSkin.AddItem("Doppler Phase 1");
	KnifeSkin.AddItem("Doppler Phase 2");
	KnifeSkin.AddItem("Doppler Phase 3");
	KnifeSkin.AddItem("Doppler Phase 4");
	KnifeSkin.AddItem("Gamma Doppler Phase 1");
	KnifeSkin.AddItem("Gamma Doppler Phase 2");
	KnifeSkin.AddItem("Gamma Doppler Phase 3");
	KnifeSkin.AddItem("Gamma Doppler Phase 4");
	KnifeSkin.AddItem("Gamma Doppler Emerald");
	KnifeSkin.AddItem("Lore");
	KnifeGroup.PlaceLabledControl(2,"Knife Skin", this, &KnifeSkin);
	//tab3

	AK47Skin.SetFileId("ak47_skin");
	AK47Skin.AddItem("First Class");
	AK47Skin.AddItem("Red Laminate");
	AK47Skin.AddItem("Case Hardened");
	AK47Skin.AddItem("Black Laminate");
	AK47Skin.AddItem("Fire Serpent");
	AK47Skin.AddItem("Cartel");
	AK47Skin.AddItem("Emerald Pinstripe");
	AK47Skin.AddItem("Blue Laminate");
	AK47Skin.AddItem("Redline");
	AK47Skin.AddItem("Vulcan");
	AK47Skin.AddItem("Jaguar");
	AK47Skin.AddItem("Jet Set");
	AK47Skin.AddItem("Wasteland Rebel");
	AK47Skin.AddItem("Elite Build");
	AK47Skin.AddItem("Hydroponic");
	AK47Skin.AddItem("Aquamarine Revenge");
	AK47Skin.AddItem("Frontside Misty");
	AK47Skin.AddItem("Point Disarray");
	AK47Skin.AddItem("Fuel Injector");
	AK47Skin.AddItem("Neon Revolution");
	AK47Skin.AddItem("BloodSport");
	AK47Skin.AddItem("Orbit Mk01");
	KnifeGroup.PlaceLabledControl(3,"AK-47", this, &AK47Skin);

	M41SSkin.SetFileId("m4a1s_skin");
	M41SSkin.AddItem("Dark Water");
	M41SSkin.AddItem("Hyper Beast");
	M41SSkin.AddItem("Boreal Forest");
	M41SSkin.AddItem("VariCamo");
	M41SSkin.AddItem("Nitro");
	M41SSkin.AddItem("Bright Water");
	M41SSkin.AddItem("Atomic Alloy");
	M41SSkin.AddItem("Blood Tiger");
	M41SSkin.AddItem("Guardian");
	M41SSkin.AddItem("Master Piece");
	M41SSkin.AddItem("Knight");
	M41SSkin.AddItem("Cyrex");
	M41SSkin.AddItem("Basilisk");
	M41SSkin.AddItem("Icarus Fell");
	M41SSkin.AddItem("Hot Rod");
	M41SSkin.AddItem("Golden Coi");
	M41SSkin.AddItem("Chantico's Fire");
	M41SSkin.AddItem("Mecha Industries");
	M41SSkin.AddItem("Flashback");
	M41SSkin.AddItem("Decimator");
	M41SSkin.AddItem("Briefing");
	KnifeGroup.PlaceLabledControl(3,"M4A1-S", this, &M41SSkin);

	M4A4Skin.SetFileId("m4a4_skin");
	M4A4Skin.AddItem("Bullet Rain");
	M4A4Skin.AddItem("Zirka");
	M4A4Skin.AddItem("Asiimov");
	M4A4Skin.AddItem("Howl");
	M4A4Skin.AddItem("X-Ray");
	M4A4Skin.AddItem("Desert-Strike");
	M4A4Skin.AddItem("Griffin");
	M4A4Skin.AddItem("Dragon King");
	M4A4Skin.AddItem("Poseidon");
	M4A4Skin.AddItem("Daybreak");
	M4A4Skin.AddItem("Evil Daimyo");
	M4A4Skin.AddItem("Royal Paladin");
	M4A4Skin.AddItem("The BattleStar");
	M4A4Skin.AddItem("Desolate Space");
	M4A4Skin.AddItem("Buzz Kill");
	M4A4Skin.AddItem("Hell Fire");
	KnifeGroup.PlaceLabledControl(3,"M4A4", this, &M4A4Skin);

	AUGSkin.SetFileId("aug_skin");
	AUGSkin.AddItem("Bengal Tiger");
	AUGSkin.AddItem("Hot Rod");
	AUGSkin.AddItem("Chameleon");
	AUGSkin.AddItem("Torque");
	AUGSkin.AddItem("Radiation Hazard");
	AUGSkin.AddItem("Asterion");
	AUGSkin.AddItem("Daedalus");
	AUGSkin.AddItem("Akihabara Accept");
	AUGSkin.AddItem("Ricochet");
	AUGSkin.AddItem("Fleet Flock");
	AUGSkin.AddItem("Syd Mead");
	KnifeGroup.PlaceLabledControl(3,"AUG", this, &AUGSkin);

	FAMASSkin.SetFileId("famas_skin");
	FAMASSkin.AddItem("Contrast Spray");
	FAMASSkin.AddItem("Colony");
	FAMASSkin.AddItem("Cyanospatter");
	FAMASSkin.AddItem("Djinn");
	FAMASSkin.AddItem("Afterimage");
	FAMASSkin.AddItem("Doomkitty");
	FAMASSkin.AddItem("Spitfire");
	FAMASSkin.AddItem("Teardown");
	FAMASSkin.AddItem("Hexane");
	FAMASSkin.AddItem("Pulse");
	FAMASSkin.AddItem("Sergeant");
	FAMASSkin.AddItem("Styx");
	FAMASSkin.AddItem("Neural Net");
	FAMASSkin.AddItem("Survivor");
	FAMASSkin.AddItem("Valence");
	FAMASSkin.AddItem("Roll Cage");
	FAMASSkin.AddItem("Mecha Industries");
	FAMASSkin.AddItem("Macabre");
	KnifeGroup.PlaceLabledControl(3,"FAMAS", this, &FAMASSkin);



	//tab8
	MAG7Skin.SetFileId("mag7_skin");
	MAG7Skin.AddItem("Counter Terrace");
	MAG7Skin.AddItem("Metallic DDPAT");
	MAG7Skin.AddItem("Silver");
	MAG7Skin.AddItem("Storm");
	MAG7Skin.AddItem("Bulldozer");
	MAG7Skin.AddItem("Heat");
	MAG7Skin.AddItem("Sand Dune");
	MAG7Skin.AddItem("Irradiated Alert");
	MAG7Skin.AddItem("Memento");
	MAG7Skin.AddItem("Hazard");
	MAG7Skin.AddItem("Heaven Guard");
	MAG7Skin.AddItem("Firestarter");
	MAG7Skin.AddItem("Seabird");
	MAG7Skin.AddItem("Cobalt Core");
	MAG7Skin.AddItem("Praetorian");
	MAG7Skin.AddItem("Hard Water");
	KnifeGroup.PlaceLabledControl(8,"Mag-7", this, &MAG7Skin);

	XM1014Skin.SetFileId("xm1014_skin");
	XM1014Skin.AddItem("Blaze Orange");
	XM1014Skin.AddItem("VariCamo Blue");
	XM1014Skin.AddItem("Bone Mask");
	XM1014Skin.AddItem("Blue Steel");
	XM1014Skin.AddItem("Blue Spruce");
	XM1014Skin.AddItem("Grassland");
	XM1014Skin.AddItem("Urban Perforated");
	XM1014Skin.AddItem("Jungle");
	XM1014Skin.AddItem("VariCamo");
	XM1014Skin.AddItem("VariCamo");
	XM1014Skin.AddItem("Fallout Warning");
	XM1014Skin.AddItem("Jungle");
	XM1014Skin.AddItem("CaliCamo");
	XM1014Skin.AddItem("Pit Viper");
	XM1014Skin.AddItem("Tranquility");
	XM1014Skin.AddItem("Red Python");
	XM1014Skin.AddItem("Heaven Guard");
	XM1014Skin.AddItem("Red Leather");
	XM1014Skin.AddItem("Bone Machine");
	XM1014Skin.AddItem("Quicksilver");
	XM1014Skin.AddItem("Scumbria");
	XM1014Skin.AddItem("Teclu Burner");
	XM1014Skin.AddItem("Black Tie");
	XM1014Skin.AddItem("Seasons");
	KnifeGroup.PlaceLabledControl(8,"XM1014", this, &XM1014Skin);

	SAWEDOFFSkin.SetFileId("sawedoff_skin");
	SAWEDOFFSkin.AddItem("First Class");
	SAWEDOFFSkin.AddItem("Forest DDPAT");
	SAWEDOFFSkin.AddItem("Contrast Spray");
	SAWEDOFFSkin.AddItem("Snake Camo");
	SAWEDOFFSkin.AddItem("Orange DDPAT");
	SAWEDOFFSkin.AddItem("Fade");
	SAWEDOFFSkin.AddItem("Copper");
	SAWEDOFFSkin.AddItem("Origami");
	SAWEDOFFSkin.AddItem("Sage Spray");
	SAWEDOFFSkin.AddItem("VariCamo");
	SAWEDOFFSkin.AddItem("Irradiated Alert");
	SAWEDOFFSkin.AddItem("Mosaico");
	SAWEDOFFSkin.AddItem("Serenity");
	SAWEDOFFSkin.AddItem("Amber Fade");
	SAWEDOFFSkin.AddItem("Full Stop");
	SAWEDOFFSkin.AddItem("Highwayman");
	SAWEDOFFSkin.AddItem("The Kraken");
	SAWEDOFFSkin.AddItem("Rust Coat");
	SAWEDOFFSkin.AddItem("Bamboo Shadow");
	SAWEDOFFSkin.AddItem("Bamboo Forest");
	SAWEDOFFSkin.AddItem("Yorick");
	SAWEDOFFSkin.AddItem("Fubar");
	SAWEDOFFSkin.AddItem("Wasteland Princess");
	SAWEDOFFSkin.AddItem("Zander");
	KnifeGroup.PlaceLabledControl(8,"Sawed-Off", this, &SAWEDOFFSkin);

	NOVASkin.SetFileId("nova_skin");
	NOVASkin.AddItem("Candy Apple");
	NOVASkin.AddItem("Blaze Orange");
	NOVASkin.AddItem("Modern Hunter");
	NOVASkin.AddItem("Forest Leaves");
	NOVASkin.AddItem("Bloomstick");
	NOVASkin.AddItem("Sand Dune");
	NOVASkin.AddItem("Polar Mesh");
	NOVASkin.AddItem("Walnut");
	NOVASkin.AddItem("Predator");
	NOVASkin.AddItem("Tempest");
	NOVASkin.AddItem("Graphite");
	NOVASkin.AddItem("Ghost Camo");
	NOVASkin.AddItem("Rising Skull");
	NOVASkin.AddItem("Antique");
	NOVASkin.AddItem("Green Apple");
	NOVASkin.AddItem("Caged Steel");
	NOVASkin.AddItem("Koi");
	NOVASkin.AddItem("Moon in Libra");
	NOVASkin.AddItem("Ranger");
	NOVASkin.AddItem("HyperBeast");
	KnifeGroup.PlaceLabledControl(8,"Nova", this, &NOVASkin);
	//tab7
	GALILSkin.SetFileId("galil_skin");
	GALILSkin.AddItem("Forest DDPAT");
	GALILSkin.AddItem("Contrast Spray");
	GALILSkin.AddItem("Orange DDPAT");
	GALILSkin.AddItem("Eco");
	GALILSkin.AddItem("Winter Forest");
	GALILSkin.AddItem("Sage Spray");
	GALILSkin.AddItem("VariCamo");
	GALILSkin.AddItem("VariCamo");
	GALILSkin.AddItem("Chatterbox");
	GALILSkin.AddItem("Shattered");
	GALILSkin.AddItem("Kami");
	GALILSkin.AddItem("Blue Titanium");
	GALILSkin.AddItem("Urban Rubble");
	GALILSkin.AddItem("Hunting Blind");
	GALILSkin.AddItem("Sandstorm");
	GALILSkin.AddItem("Tuxedo");
	GALILSkin.AddItem("Cerberus");
	GALILSkin.AddItem("Aqua Terrace");
	GALILSkin.AddItem("Rocket Pop");
	GALILSkin.AddItem("Stone Cold");
	GALILSkin.AddItem("Firefight");
	GALILSkin.AddItem("Crimson Tsunami");
	GALILSkin.AddItem("Suger Rush");
	KnifeGroup.PlaceLabledControl(7, "GALIL", this, &GALILSkin);

	SG553Skin.SetFileId("sg552_skin");
	SG553Skin.AddItem("Bulldozer");
	SG553Skin.AddItem("Ultraviolet");
	SG553Skin.AddItem("Damascus Steel");
	SG553Skin.AddItem("Fallout Warning");
	SG553Skin.AddItem("Damascus Steel");
	SG553Skin.AddItem("Pulse");
	SG553Skin.AddItem("Army Sheen");
	SG553Skin.AddItem("Traveler");
	SG553Skin.AddItem("Fallout Warning");
	SG553Skin.AddItem("Cyrex");
	SG553Skin.AddItem("Tiger Moth");
	SG553Skin.AddItem("Atlas");
	KnifeGroup.PlaceLabledControl(7, "SG552", this, &SG553Skin);

	NEGEVSkin.SetFileId("negev_skin");
	NEGEVSkin.AddItem("Anodized Navy");
	NEGEVSkin.AddItem("Man-o'-war");
	NEGEVSkin.AddItem("Palm");
	NEGEVSkin.AddItem("VariCamo");
	NEGEVSkin.AddItem("Palm");
	NEGEVSkin.AddItem("CaliCamo");
	NEGEVSkin.AddItem("Terrain");
	NEGEVSkin.AddItem("Army Sheen");
	NEGEVSkin.AddItem("Bratatat");
	NEGEVSkin.AddItem("Desert-Strike");
	NEGEVSkin.AddItem("Nuclear Waste");
	NEGEVSkin.AddItem("Loudmouth");
	NEGEVSkin.AddItem("Power Loader");
	KnifeGroup.PlaceLabledControl(7,"Negev", this, &NEGEVSkin);

	M249Skin.SetFileId("m249_skin");
	M249Skin.AddItem("Contrast Spray");
	M249Skin.AddItem("Blizzard Marbleized");
	M249Skin.AddItem("Jungle DDPAT");
	M249Skin.AddItem("Gator Mesh");
	M249Skin.AddItem("Magma");
	M249Skin.AddItem("System Lock");
	M249Skin.AddItem("Shipping Forecast");
	M249Skin.AddItem("Impact Drill");
	M249Skin.AddItem("Nebula Crusader");
	M249Skin.AddItem("Spectre");
	M249Skin.AddItem("Emerald Poison Dart");
	KnifeGroup.PlaceLabledControl(7,"M249", this, &M249Skin);

	//tab4
	AWPSkin.SetFileId("awp_skin");
	AWPSkin.AddItem("BOOM");
	AWPSkin.AddItem("Dragon Lore");
	AWPSkin.AddItem("Pink DDPAT");
	AWPSkin.AddItem("Snake Camo");
	AWPSkin.AddItem("Lightning Strike");
	AWPSkin.AddItem("Safari Mesh");
	AWPSkin.AddItem("Corticera");
	AWPSkin.AddItem("Redline");
	AWPSkin.AddItem("Man-o'-war");
	AWPSkin.AddItem("Graphite");
	AWPSkin.AddItem("Electric Hive");
	AWPSkin.AddItem("Pit Viper");
	AWPSkin.AddItem("Asiimov");
	AWPSkin.AddItem("Worm God");
	AWPSkin.AddItem("Medusa");
	AWPSkin.AddItem("Sun in Leo");
	AWPSkin.AddItem("Hyper Beast");
	AWPSkin.AddItem("Elite Build");
	AWPSkin.AddItem("Fever Dream");
	AWPSkin.AddItem("Oni Taiji");
	KnifeGroup.PlaceLabledControl(4, "AWP", this, &AWPSkin);

	SSG08Skin.SetFileId("sgg08_skin");
	SSG08Skin.AddItem("Lichen Dashed");
	SSG08Skin.AddItem("Dark Water");
	SSG08Skin.AddItem("Blue Spruce");
	SSG08Skin.AddItem("Sand Dune");
	SSG08Skin.AddItem("Palm");
	SSG08Skin.AddItem("Mayan Dreams");
	SSG08Skin.AddItem("Blood in the Water");
	SSG08Skin.AddItem("Tropical Storm");
	SSG08Skin.AddItem("Acid Fade");
	SSG08Skin.AddItem("Slashed");
	SSG08Skin.AddItem("Detour");
	SSG08Skin.AddItem("Abyss");
	SSG08Skin.AddItem("Big Iron");
	SSG08Skin.AddItem("Necropos");
	SSG08Skin.AddItem("Ghost Crusader");
	SSG08Skin.AddItem("Dragonfire");
	SSG08Skin.AddItem("Death's Head");
	KnifeGroup.PlaceLabledControl(4, "SGG 08", this, &SSG08Skin);

	SCAR20Skin.SetFileId("scar20_skin");
	SCAR20Skin.AddItem("Splash Jam");
	SCAR20Skin.AddItem("Storm");
	SCAR20Skin.AddItem("Contractor");
	SCAR20Skin.AddItem("Carbon Fiber");
	SCAR20Skin.AddItem("Sand Mesh");
	SCAR20Skin.AddItem("Palm");
	SCAR20Skin.AddItem("Emerald");
	SCAR20Skin.AddItem("Crimson Web");
	SCAR20Skin.AddItem("Cardiac");
	SCAR20Skin.AddItem("Army Sheen");
	SCAR20Skin.AddItem("Cyrex");
	SCAR20Skin.AddItem("Grotto");
	SCAR20Skin.AddItem("Emerald");
	SCAR20Skin.AddItem("Green Marine");
	SCAR20Skin.AddItem("Outbreak");
	SCAR20Skin.AddItem("Bloodsport");
	SCAR20Skin.AddItem("Blueprint");
	KnifeGroup.PlaceLabledControl(4, "SCAR-20", this, &SCAR20Skin);

	G3SG1Skin.SetFileId("g3sg1_skin");
	G3SG1Skin.AddItem("Desert Storm");
	G3SG1Skin.AddItem("Arctic Camo");
	G3SG1Skin.AddItem("Bone Mask");
	G3SG1Skin.AddItem("Contractor");
	G3SG1Skin.AddItem("Safari Mesh");
	G3SG1Skin.AddItem("Polar Camo");
	G3SG1Skin.AddItem("Jungle Dashed");
	G3SG1Skin.AddItem("VariCamo");
	G3SG1Skin.AddItem("Predator");
	G3SG1Skin.AddItem("Demeter");
	G3SG1Skin.AddItem("Azure Zebra");
	G3SG1Skin.AddItem("Green Apple");
	G3SG1Skin.AddItem("Orange Kimono");
	G3SG1Skin.AddItem("Neon Kimono");
	G3SG1Skin.AddItem("Murky");
	G3SG1Skin.AddItem("Chronos");
	G3SG1Skin.AddItem("Flux");
	G3SG1Skin.AddItem("The Executioner");
	G3SG1Skin.AddItem("Orange Crash");
	KnifeGroup.PlaceLabledControl(4, "G3SG1", this, &G3SG1Skin);


	//tab5
	GLOCKSkin.SetFileId("glock_skin");
	GLOCKSkin.AddItem("Groundwater");
	GLOCKSkin.AddItem("Candy Apple");
	GLOCKSkin.AddItem("Fade");
	GLOCKSkin.AddItem("Night");
	GLOCKSkin.AddItem("Dragon Tattoo");
	GLOCKSkin.AddItem("Twilight Galaxy");
	GLOCKSkin.AddItem("Sand Dune");
	GLOCKSkin.AddItem("Brass");
	GLOCKSkin.AddItem("Catacombs");
	GLOCKSkin.AddItem("Sand Dune");
	GLOCKSkin.AddItem("Steel Disruption");
	GLOCKSkin.AddItem("Blue Fissure");
	GLOCKSkin.AddItem("Death Rattle");
	GLOCKSkin.AddItem("Water Elemental");
	GLOCKSkin.AddItem("Reactor");
	GLOCKSkin.AddItem("Grinder");
	GLOCKSkin.AddItem("Bunsen Burner");
	GLOCKSkin.AddItem("Wraith");
	GLOCKSkin.AddItem("Royal Legion");
	GLOCKSkin.AddItem("Weasel");
	GLOCKSkin.AddItem("Wasteland Rebel");
	KnifeGroup.PlaceLabledControl(5, "Glock", this, &GLOCKSkin);

	USPSSkin.SetFileId("usps_skin");
	USPSSkin.AddItem("Forest Leaves");
	USPSSkin.AddItem("Dark Water");
	USPSSkin.AddItem("VariCamo");
	USPSSkin.AddItem("Overgrowth");
	USPSSkin.AddItem("Caiman");
	USPSSkin.AddItem("Blood Tiger");
	USPSSkin.AddItem("Serum");
	USPSSkin.AddItem("Night Ops");
	USPSSkin.AddItem("Stainless");
	USPSSkin.AddItem("Guardian");
	USPSSkin.AddItem("Orion");
	USPSSkin.AddItem("Road Rash");
	USPSSkin.AddItem("Royal Blue");
	USPSSkin.AddItem("Business Class");
	USPSSkin.AddItem("Para Green");
	USPSSkin.AddItem("Torque");
	USPSSkin.AddItem("Kill Confirmed");
	USPSSkin.AddItem("Lead Conduit");
	USPSSkin.AddItem("Cyrex");
	USPSSkin.AddItem("Neo-Noir");
	USPSSkin.AddItem("Blueprint");
	KnifeGroup.PlaceLabledControl(5, "USP-S", this, &USPSSkin);

	DEAGLESkin.SetFileId("deagle_skin");
	DEAGLESkin.AddItem("Blaze");
	DEAGLESkin.AddItem("Pilot");
	DEAGLESkin.AddItem("Midnight Storm");
	DEAGLESkin.AddItem("Sunset Storm");
	DEAGLESkin.AddItem("Forest DDPAT");
	DEAGLESkin.AddItem("Crimson Web");
	DEAGLESkin.AddItem("Urban DDPAT");
	DEAGLESkin.AddItem("Night");
	DEAGLESkin.AddItem("Hypnotic");
	DEAGLESkin.AddItem("Mudder");
	DEAGLESkin.AddItem("VariCamo");
	DEAGLESkin.AddItem("Golden Koi");
	DEAGLESkin.AddItem("Cobalt Disruption");
	DEAGLESkin.AddItem("Urban Rubble");
	DEAGLESkin.AddItem("Naga");
	DEAGLESkin.AddItem("Hand Cannon");
	DEAGLESkin.AddItem("Heirloom");
	DEAGLESkin.AddItem("Meteorite");
	DEAGLESkin.AddItem("Conspiracy");
	DEAGLESkin.AddItem("Bronze Deco");
	DEAGLESkin.AddItem("Sunset Storm");
	DEAGLESkin.AddItem("Corinthian");
	DEAGLESkin.AddItem("Kumicho Dragon");
	DEAGLESkin.AddItem("Oxide Blaze");
	KnifeGroup.PlaceLabledControl(5, "Deagle", this, &DEAGLESkin);

	DUALSSkin.SetFileId("duals_skin");
	DUALSSkin.AddItem("Anodized Navy");
	DUALSSkin.AddItem("Stained");
	DUALSSkin.AddItem("Contractor");
	DUALSSkin.AddItem("Colony");
	DUALSSkin.AddItem("Demolition");
	DUALSSkin.AddItem("Black Limba");
	DUALSSkin.AddItem("Cobalt Quartz");
	DUALSSkin.AddItem("Hemoglobin");
	DUALSSkin.AddItem("Urban Shock");
	DUALSSkin.AddItem("Marina");
	DUALSSkin.AddItem("Panther");
	DUALSSkin.AddItem("Retribution");
	DUALSSkin.AddItem("Briar");
	DUALSSkin.AddItem("Duelist");
	DUALSSkin.AddItem("Moon in Libra");
	DUALSSkin.AddItem("Dualing Dragons");
	DUALSSkin.AddItem("Cartel");
	DUALSSkin.AddItem("Ventilators");
	DUALSSkin.AddItem("Cobra Strike");
	KnifeGroup.PlaceLabledControl(5, "Duals", this, &DUALSSkin);

	FIVESEVENSkin.SetFileId("fiveseven_skin");
	FIVESEVENSkin.AddItem("Candy Apple");
	FIVESEVENSkin.AddItem("Bone Mask");
	FIVESEVENSkin.AddItem("Case Hardened");
	FIVESEVENSkin.AddItem("Contractor");
	FIVESEVENSkin.AddItem("Forest Night");
	FIVESEVENSkin.AddItem("Orange Peel");
	FIVESEVENSkin.AddItem("Jungle");
	FIVESEVENSkin.AddItem("Nitro");
	FIVESEVENSkin.AddItem("Red Quartz");
	FIVESEVENSkin.AddItem("Anodized Gunmetal");
	FIVESEVENSkin.AddItem("Nightshade");
	FIVESEVENSkin.AddItem("Silver Quartz");
	FIVESEVENSkin.AddItem("Kami");
	FIVESEVENSkin.AddItem("Copper Galaxy");
	FIVESEVENSkin.AddItem("Neon Kimono");
	FIVESEVENSkin.AddItem("Fowl Play");
	FIVESEVENSkin.AddItem("Hot Shot");
	FIVESEVENSkin.AddItem("Urban Hazard");
	FIVESEVENSkin.AddItem("Monkey Business");
	FIVESEVENSkin.AddItem("Retrobution");
	FIVESEVENSkin.AddItem("Triumvirate");
	FIVESEVENSkin.AddItem("Capillary");
	FIVESEVENSkin.AddItem("Hyper Beast");
	KnifeGroup.PlaceLabledControl(5, "Five-Seven", this, &FIVESEVENSkin);

	TECNINESkin.SetFileId("tec9_skin");
	TECNINESkin.AddItem("Tornado");
	TECNINESkin.AddItem("Groundwater");
	TECNINESkin.AddItem("Forest DDPAT");
	TECNINESkin.AddItem("Terrace");
	TECNINESkin.AddItem("Urban DDPAT");
	TECNINESkin.AddItem("Ossified");
	TECNINESkin.AddItem("Hades");
	TECNINESkin.AddItem("Brass");
	TECNINESkin.AddItem("VariCamo");
	TECNINESkin.AddItem("Nuclear Threat");
	TECNINESkin.AddItem("Red Quartz");
	TECNINESkin.AddItem("Tornado");
	TECNINESkin.AddItem("Blue Titanium");
	TECNINESkin.AddItem("Army Mesh");
	TECNINESkin.AddItem("Titanium Bit");
	TECNINESkin.AddItem("Sandstorm");
	TECNINESkin.AddItem("Isaac");
	TECNINESkin.AddItem("Toxic");
	TECNINESkin.AddItem("Bamboo Forest");
	TECNINESkin.AddItem("Avalanche");
	TECNINESkin.AddItem("Jambiya");
	TECNINESkin.AddItem("Re-Entry");
	TECNINESkin.AddItem("Fuel Injector");
	TECNINESkin.AddItem("Cut Out");
	KnifeGroup.PlaceLabledControl(5, "Tec-9", this, &TECNINESkin);


	//tab 6

	P2000Skin.SetFileId("p2000_skin");
	P2000Skin.AddItem("Grassland Leaves");
	P2000Skin.AddItem("Silver");
	P2000Skin.AddItem("Granite Marbleized");
	P2000Skin.AddItem("Forest Leaves");
	P2000Skin.AddItem("Ossified");
	P2000Skin.AddItem("Handgun");
	P2000Skin.AddItem("Fade");
	P2000Skin.AddItem("Scorpion");
	P2000Skin.AddItem("Grassland");
	P2000Skin.AddItem("Corticera");
	P2000Skin.AddItem("Ocean Foam");
	P2000Skin.AddItem("Pulse");
	P2000Skin.AddItem("Amber Fade");
	P2000Skin.AddItem("Red FragCam");
	P2000Skin.AddItem("Chainmail");
	P2000Skin.AddItem("Coach Class");
	P2000Skin.AddItem("Ivory");
	P2000Skin.AddItem("Fire Elemental");
	P2000Skin.AddItem("Asterion");
	P2000Skin.AddItem("Pathfinder");
	P2000Skin.AddItem("Imperial");
	P2000Skin.AddItem("Oceanic");
	P2000Skin.AddItem("Imperial Dragon");
	P2000Skin.AddItem("Woodsman");
	KnifeGroup.PlaceLabledControl(5, "P2000", this, &P2000Skin);

	P250Skin.SetFileId("p250_skin");
	P250Skin.AddItem("Whiteout");
	P250Skin.AddItem("Metallic DDPAT");
	P250Skin.AddItem("Splash");
	P250Skin.AddItem("Gunsmoke");
	P250Skin.AddItem("Modern Hunter");
	P250Skin.AddItem("Bone Mask");
	P250Skin.AddItem("Boreal Forest");
	P250Skin.AddItem("Sand Dune");
	P250Skin.AddItem("Nuclear Threat");
	P250Skin.AddItem("Mehndi");
	P250Skin.AddItem("Facets");
	P250Skin.AddItem("Hive");
	P250Skin.AddItem("Muertos");
	P250Skin.AddItem("Steel Disruption");
	P250Skin.AddItem("Undertow");
	P250Skin.AddItem("Franklin");
	P250Skin.AddItem("Neon Kimono");
	P250Skin.AddItem("Supernova");
	P250Skin.AddItem("Contamination");
	P250Skin.AddItem("Cartel");
	P250Skin.AddItem("Valence");
	P250Skin.AddItem("Crimson Kimono");
	P250Skin.AddItem("Mint Kimono");
	P250Skin.AddItem("Wing Shot");
	P250Skin.AddItem("Asiimov");
	P250Skin.AddItem("Ripple");
	P250Skin.AddItem("Red Rock");
	KnifeGroup.PlaceLabledControl(6, "P250", this, &P250Skin);

	CZSkin.SetFileId("cz_skin");
	CZSkin.AddItem("Victoria");
	CZSkin.AddItem("Crimson Web");
	CZSkin.AddItem("Yellow Jacket");
	CZSkin.AddItem("Fuschia");
	CZSkin.AddItem("Xiangliu");
	CZSkin.AddItem("Pole Position");
	CZSkin.AddItem("Tigris");
	CZSkin.AddItem("Red Astor");
	CZSkin.AddItem("Tread Plate");
	CZSkin.AddItem("Chalice");
	CZSkin.AddItem("Imprint");
	CZSkin.AddItem("Twist");
	CZSkin.AddItem("Polymer");
	CZSkin.AddItem("Hexane");
	CZSkin.AddItem("Tuxedo");
	CZSkin.AddItem("Nitro");
	CZSkin.AddItem("Emerald");
	CZSkin.AddItem("Poison Dart");
	KnifeGroup.PlaceLabledControl(6, "CZ75", this, &CZSkin);

	r8skin.SetFileId("r8");
	r8skin.AddItem("Crimson Web");
	r8skin.AddItem("Fade");
	r8skin.AddItem("Reboot");
	r8skin.AddItem("AmberFade");
	r8skin.AddItem("Dry Season");
	KnifeGroup.PlaceLabledControl(6, "R8-Revolver", this, &r8skin);

#pragma endregion



#pragma endregion other random options
}

void CSettingsTab::Setup()
{
SetTitle("PlayerList");


#pragma region Options

Options.SetPosition(16, 350);
Options.SetSize(150, 120);
Options.SetText("Player Options");
RegisterControl(&Options);

nospreadresolver.SetFileId("resolvernospread");
Options.PlaceLabledControl("Resolver NoSpread", this, &nospreadresolver);



CustomNospreadResolver.SetFileId("customnspredres");
Options.PlaceLabledControl("Custom->", this, &CustomNospreadResolver);

#pragma endregion

#pragma region hvh

hvh.SetPosition(170, 350);
hvh.SetSize(200, 235);
hvh.SetText("Player");
RegisterControl(&hvh);

Bruteforce1.SetFileId("resolverone");
Bruteforce1.SetBoundaries(0, 360);
Bruteforce1.SetValue(0);
hvh.PlaceLabledControl("BruteAngle1", this, &Bruteforce1);

Bruteforce2.SetFileId("resolvertwo");
Bruteforce2.SetBoundaries(0, 360);
Bruteforce2.SetValue(0);
hvh.PlaceLabledControl("BruteAngle2", this, &Bruteforce2);

Bruteforce3.SetFileId("resolverthree");
Bruteforce3.SetBoundaries(0, 360);
Bruteforce3.SetValue(0);
hvh.PlaceLabledControl("BruteAngle3", this, &Bruteforce3);

Bruteforce4.SetFileId("resolverfour");
Bruteforce4.SetBoundaries(0, 360);
Bruteforce4.SetValue(0);
hvh.PlaceLabledControl("BruteAngle4", this, &Bruteforce4);

Bruteforce5.SetFileId("resolverfive");
Bruteforce5.SetBoundaries(0, 360);
Bruteforce5.SetValue(0);
hvh.PlaceLabledControl("BruteAngle5", this, &Bruteforce5);

#pragma endregion

#pragma region hvh2

hvh2.SetPosition(350, 350);
hvh2.SetSize(200, 230);
hvh2.SetText("Player Options");
RegisterControl(&hvh2);

Bruteforce6.SetFileId("resolversix");
Bruteforce6.SetBoundaries(0, 360);
Bruteforce6.SetValue(0);
hvh2.PlaceLabledControl("BruteAngle6", this, &Bruteforce6);

Bruteforce7.SetFileId("resolverseven");
Bruteforce7.SetBoundaries(0, 360);
Bruteforce7.SetValue(0);
hvh2.PlaceLabledControl("BruteAngle7", this, &Bruteforce7);

Bruteforce8.SetFileId("resolvereight");
Bruteforce8.SetBoundaries(0, 360);
Bruteforce8.SetValue(0);
hvh2.PlaceLabledControl("BruteAngle8", this, &Bruteforce8);

Bruteforce9.SetFileId("resolvernein");
Bruteforce9.SetBoundaries(0, 360);
Bruteforce9.SetValue(0);
hvh2.PlaceLabledControl("BruteAngle9", this, &Bruteforce9);

Bruteforce10.SetFileId("resolverten");
Bruteforce10.SetBoundaries(0, 360);
Bruteforce10.SetValue(0);
hvh2.PlaceLabledControl("BruteAngle10", this, &Bruteforce10);

#pragma endregion
}

void Menu::SetupMenu()
{
	Window.Setup();

	GUI.RegisterWindow(&Window);
	GUI.BindWindow(VK_INSERT, &Window);
}

void Menu::DoUIFrame()
{


	GUI.Update();
	GUI.Draw();
	
}

