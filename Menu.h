#pragma once

#include "GUI.h"
#include "Controls.h"




class CRageBotTab : public CTab
{
public:
	void Setup();


	//custom
	CCheckBox Hitboxhead;
	CCheckBox Hitboxneck;
	CCheckBox Hitboxchest;
	CCheckBox Hitboxarms;
	CCheckBox Hitboxstomach;
	CCheckBox Hitboxlegs;






	CLabel ActiveLabel;
	CCheckBox Active;

	CGroupBox hitchance;
	CGroupBox mindmg;




	CGroupBox AimbotGroup;
	CCheckBox AimbotEnable;
	CCheckBox SWSwitchEnable;
	CCheckBox Freestanding;
	CKeyBind SWSwitchKey;
	CCheckBox AimbotAutoFire;
	CCheckBox AccuracyBacktracking;
	CSlider  AimbotFov;
	CCheckBox AimbotSilentAim;
	CCheckBox PVSFix;
	CCheckBox AimbotEnginePred;
	CKeyBind AimbotBaimOnKey;
	CComboBox AimbotResolver;
	CCheckBox AccuracyResolverbrute;
	CCheckBox PitchAimbotResolver;
	CCheckBox AutoRevolver;
	CSlider AddFakeYaw;
	CSlider CustomPitch;
	CCheckBox LBY;
	CComboBox FakeYaw;   
	CSlider Antilby;
	CSlider LowerBodyBreakerYawAdd;


	CCheckBox lbytoggleAA;

	CComboBox InAir;
	CComboBox MoveYawFake;
	CCheckBox LowerbodyFix;
	CCheckBox PreferBodyAim;
	CCheckBox AWPAtBody;
	CSlider BaimIfUnderXHealth;
	CSlider XShots;
	CCheckBox EnginePrediction;
	CCheckBox AimbotAimStep;
	CCheckBox AimbotKeyPress;
	CKeyBind  AimbotKeyBind;
	CKeyBind  AimbotStopKey;

	CGroupBox TargetGroup;
	CComboBox TargetSelection;
	CCheckBox TargetFriendlyFire;
	CComboBox TargetHitbox;
	CComboBox TargetHitscan;
	CCheckBox TargetMultipoint;
	CSlider   TargetPointscale;

	CGroupBox AccuracyGroup;
	CCheckBox AccuracyRecoil;
	CCheckBox AccuracyAutoWall;
	CSlider AccuracyMinimumDamage;
	CSlider Fakelagvalue;
	CCheckBox AccuracyAutoStop;
	CCheckBox AccuracyAutoCrouch;
	CCheckBox AccuracyAutoScope;
	CCheckBox FakeLagEnable;
	CSlider   AccuracyHitchance;
	CCheckBox AccuracyResolver;

	CCheckBox WeaponCheck;
	CSlider  scoutChance;
	CSlider  AWPChance;
	CSlider  AutoChance;
	CSlider  RifleChance;
	CSlider  MPChance;
	CSlider  PistolChance;
	CSlider  R8Chance;


	CSlider  scoutmindmg;
	CSlider  AWPmindmg;
	CSlider  Automindmg;
	CSlider  Riflemindmg;
	CSlider  MPmindmg;
	CSlider  Pistolmindmg;
	CSlider  R8mindmg;

	CMultiBox hitboxes;

	CGroupBox AntiAimGroup;
	CCheckBox AntiAimEnable;
	CComboBox AntiAimPitch;
	CComboBox AntiAimYaw;
	CKeyBind ManualLeft;
	CKeyBind ManualRight;
	CKeyBind ManualBack;
	CCheckBox FlipAA;
	CComboBox BreakLBY;
	CComboBox AntiAimEdge;
	CSlider AntiAimOffset;
	CCheckBox AntiAimKnife;
	CCheckBox AntiAimTarget;
};

class CLegitBotTab : public CTab
{
public:
	void Setup();

	// Master Switch
	CLabel ActiveLabel;
	CCheckBox Active;

	CGroupBox2 AimbotGroup;


	CKeyBind legitaaswitch;


	CCheckBox AimbotEnable;
	CCheckBox AimbotAutoFire;
	CSlider   TickModulation;
	CCheckBox AimbotBacktrack;
	CCheckBox AimbotFriendlyFire;
	CCheckBox AimbotKeyPress;
	CKeyBind  AimbotKeyBind;
	CComboBox legitaa;
	CCheckBox Legitaareolver;

	CCheckBox AimbotSmokeCheck;
	CCheckBox BackTrack;


	CGroupBox TriggerGroup;
	CCheckBox TriggerEnable;
	CCheckBox TriggerKeyPress;
	CKeyBind  TriggerKeyBind;
	CCheckBox TriggerHitChance;
	CSlider   TriggerHitChanceAmmount;
	CSlider   TriggerDelay;
	CSlider   TriggerBurst;
	CSlider   TriggerBreak;
	CCheckBox TriggerRecoil;

	CGroupBox TriggerFilterGroup;
	CCheckBox TriggerHead;
	CCheckBox TriggerChest;
	CCheckBox TriggerStomach;
	CCheckBox TriggerArms;
	CCheckBox TriggerLegs;
	CCheckBox TriggerTeammates;
	CCheckBox TriggerFlashCheck;
	CCheckBox TriggerSmokeCheck;

	CGroupBox WeaponMainGroup;
	CSlider   WeaponMainSpeed;
	CSlider   WeaponMainFoV;
	CSlider WeaponMainRecoil;
	CComboBox WeaponMainHitbox;
	CSlider WeaponMainAimtime;
	CSlider WeaoponMainStartAimtime;

	CGroupBox WeaponPistGroup;
	CSlider   WeaponPistSpeed;
	CSlider   WeaponPistFoV;
	CSlider WeaponPistRecoil;
	CComboBox WeaponPistHitbox;
	CSlider WeaponPistAimtime;
	CSlider WeaoponPistStartAimtime;

	CGroupBox WeaponSnipGroup;
	CSlider   WeaponSnipSpeed;
	CSlider   WeaponSnipFoV;
	CSlider WeaponSnipRecoil;
	CComboBox WeaponSnipHitbox;
	CSlider WeaponSnipAimtime;
	CSlider WeaoponSnipStartAimtime;

	CGroupBox WeaponMpGroup;
	CSlider   WeaponMpSpeed;
	CSlider   WeaponMpFoV;
	CSlider WeaponMpRecoil;
	CComboBox WeaponMpHitbox;
	CSlider WeaponMpAimtime;
	CSlider WeaoponMpStartAimtime;

	CGroupBox WeaponShotgunGroup;
	CSlider   WeaponShotgunSpeed;
	CSlider   WeaponShotgunFoV;
	CSlider WeaponShotgunRecoil;
	CComboBox WeaponShotgunHitbox;
	CSlider WeaponShotgunAimtime;
	CSlider WeaoponShotgunStartAimtime;

	CGroupBox WeaponMGGroup;
	CSlider   WeaponMGSpeed;
	CSlider   WeaponMGFoV;
	CSlider WeaponMGRecoil;
	CComboBox WeaponMGHitbox;
	CSlider WeaponMGAimtime;
	CSlider WeaoponMGStartAimtime;
};

class CVisualTab : public CTab
{
public:
	void Setup();

	CLabel ActiveLabel;
	CCheckBox Active;
	CCheckBox pLocalOpacity;
	CGroupBox OptionsGroup;
	CGroupBox InfoGroup;
	CGroupBox RemovalsGroup;
	CGroupBox ChamsGroup;
	CGroupBox BoxGroup;
	CGroupBox EffectGroup;
	CComboBox OptionsBox;
	CCheckBox OptionsName;
	CCheckBox Normalcrosshair;

	CCheckBox SpreadCrosshair;


	CCheckBox HitmarkerSound;
	CCheckBox dot;
	CCheckBox Angleinfo;
	CCheckBox Logs;
	CCheckBox OtherHitmarker;
	CCheckBox DamageIndicator;
	CCheckBox OptionsWeapon;
	CCheckBox OptionHealthEnable;
	CCheckBox OptionsInfo;
	CCheckBox LBYIdicador;
	CCheckBox ManualaaIndicator;
	CCheckBox ResolverInfo;
	CCheckBox fakes;
	CCheckBox BulletTrace;
	CCheckBox Barrels;
	CSlider BulletTraceLength;
	CCheckBox AALines;
	CCheckBox ChamsVisibleOnly;
	CCheckBox Watermark;
	CCheckBox OtherGrenadeTrajectory;
	CCheckBox otherbulletimpact;
	CCheckBox WelcomeMessage;
	CCheckBox GrenadePrediction;
	CCheckBox LocalPlayerESP;
	CCheckBox CanHit;
	CCheckBox OptionsHelmet;
	CCheckBox NightMode;
	CCheckBox OptionsKit;
	CCheckBox SpecList;
	CCheckBox IsScoped;
	CCheckBox CompRank;
	CCheckBox HasDefuser;
	CCheckBox GrenadeTrace;
	CCheckBox Distance;
	CCheckBox DiscoLights;
	CCheckBox Money;
	CCheckBox OptionsDefuse;
	CComboBox OptionsChams;
	CCheckBox OptionsArmor;
	CSlider  GlowZ;
	CCheckBox OptionsSkeleton;
	CCheckBox BacktrackingLol;
	CCheckBox OptionsAimSpot;
	CCheckBox OtherNoScope;
	CCheckBox OptionsCompRank;

	CComboBox Chams;
	CCheckBox ChamsPlayers;
	CCheckBox ChamsEnemyOnly;



	CCheckBox FiltersAll;
	CCheckBox FiltersPlayers;
	CCheckBox FiltersSelf;
	CCheckBox FiltersEnemiesOnly;
	CCheckBox FiltersWeapons;
	CCheckBox FiltersNades;
	CCheckBox FiltersC4;

	CComboBox XHairs;
	CCheckBox OptionsGlow;
	CCheckBox EntityGlow;
	CCheckBox NightSky;
	CCheckBox OtherRadar;
	CCheckBox OtherNoVisualRecoil;
	CCheckBox OtherNoSky; 
	CCheckBox OtherNoFlash; 
	CCheckBox OtherNoSmoke;
	CCheckBox OtherAsusWalls;
	CComboBox OtherNoHands;
	CCheckBox BulletTracers;
	CSlider OtherViewmodelFOV;
	CSlider OtherFOV;
};

class CColorTab : public CTab
{
public:
	void Setup();

	CGroupBox RGBGroup;
	CSlider   MenuR;
	CSlider   MenuG;
	CSlider  MenuB;

	CGroupBox Colors;
	CComboBox ColorsPicker;
	CGroupBox CTBoxNotVisGroup;
	CGroupBox ColorsGroup;
	CSlider CTColorVisR;
	CSlider CTColorVisG;
	CSlider CTColorVisB;

	CSlider TChams;
	CSlider CTChams;
	CSlider VisR;
	CSlider VisG;
	CSlider VisB;
	CSlider NotVisR;
	CSlider NotVisG;
	CSlider NotVisB;
	CSlider ColorMenuAccentR;
	CSlider ColorMenuAccentG;
	CSlider ColorMenuAccentB;

	CSlider CTColorNoVisR;
	CSlider CTColorNoVisG;
	CSlider CTColorNoVisB;
	CGroupBox TVisibleBoxGroup;
	CGroupBox Tboxnotvisiblegroup;
	CGroupBox GlowGroup;
	CLabel ColorSpacer;

	CGroupBox bullettracers;
	CSlider  BULLETCT_R;
	CSlider  BULLETCT_G;
	CSlider  BULLETCT_B;
	//CT Visible
	CSlider  BULLET_T_R;
	CSlider  BULLET_T_G;
	CSlider  BULLET_T_B;

	CSlider TColorVisR;
	CSlider TColorVisG;
	CSlider TColorVisB;

	CSlider TColorNoVisR;
	CSlider TColorNoVisG;
	CSlider TColorNoVisB;

	CSlider GlowR;
	CSlider GlowG;
	CSlider GlowB;

	CGroupBox CTCHamsGroup;
	CGroupBox TChamsGroup;
	CGroupBox MenuColorGroup;
	CComboBox ConfigBox;
	CGroupBox ConfigGroup;
	CGroupBox Settings;
	CButton SettingsSave;
	CButton SettingsLoad;
};

class CSettingsTab : public CTab
{
public:
	void Setup();

	CGroupBox grpPList;
	CListBox lstPlayers;
	CSilentGroupBox Options;
	CCheckBox PlayerFriendly;
	CCheckBox PlayerAimPrio;
	CCheckBox PlayerCalloutSpam;
	CCheckBox nospreadresolver;
	CCheckBox CustomNospreadResolver;

	CSilentGroupBox hvh;
	CSlider Bruteforce1;
	CSlider Bruteforce2;
	CSlider Bruteforce3;
	CSlider Bruteforce4;
	CSlider Bruteforce5;

	CSilentGroupBox hvh2;
	CSlider Bruteforce6;
	CSlider Bruteforce7;
	CSlider Bruteforce8;
	CSlider Bruteforce9;
	CSlider Bruteforce10;

};

class CMiscTab : public CTab
{
public:
	void Setup();

	CButtonSave SaveButton3;
	CButtonLoad LoadButton3;

	CGroupBox OtherGroup;
	CCheckBox OtherAutoJump;
	CCheckBox OtherEdgeJump;
	CComboBox OtherAutoStrafe;
	CCheckBox OtherSafeMode;
	CCheckBox ChatSpam;
	CCheckBox NoName;
	CCheckBox Namespam;
	CCheckBox OtherCircle;
	CKeyBind OtherCirclekey;
	CTextField CustomClantag;
	CCheckBox Impacts;
	CCheckBox CheatsByPass;
	CCheckBox AutoAccept;
	CCheckBox BuyBotKevlar;
	CCheckBox MoonWalk;
	CCheckBox BuyBotDefuser;
	CComboBox BuyBotGrenades;
	CSlider CircleAmount;
	CKeyBind FakeWalk;
	CComboBox FakeLagTyp;

	CComboBox lstPlayers;

	CSlider ClanTagSpeed;
	CComboBox NameChanger;
	CCheckBox AutoPistol;
	CCheckBox ClanTag;
	CCheckBox TrashTalk;
	CKeyBind  OtherAirStuck;
	CCheckBox OtherSpectators;
	CCheckBox OtherThirdperson;
	CKeyBind  ThirdPersonKeyBind;
	CSlider OtherThirdpersonRange;
	CCheckBox FakeAngleChams;
	CComboBox SeeTpangle;
	CCheckBox OtherThirdpersonFake;
	CComboBox OtherClantag;
	CComboBox ConfigBox;
	CTextField customclan;
	CCheckBox EnableBuyBot;
	CComboBox BuyBot;
	CCheckBox DisablePostProcess;
	CGroupBox BuyBotGroup;
	CSlider WalkBotDistance;
	CSlider WalkBotEdge;
	CCheckBox WalkBot;
	CGroupBox WalkBotGroup;
	CGroupBox FakeLagGroup;
	CCheckBox FakeLagEnable;
	CSlider   FakeLagChoke;
	CComboBox KnifeModel;
	CCheckBox SkinEnable;
	CTextField KnifeSkin;
	CGroupBox KnifeGroup;
	CButton KnifeApply;
};
class SkinsTab : public CTab
{
public:
	void Setup();

	// Knife Changer/Skin Changer
	CLabel SkinActive;
	CCheckBox SkinEnable;
	CButtonApplySkin   SkinApply;

	// Knife
	CGroupBox KnifeGroup;
	CComboBox KnifeModel;
	CComboBox KnifeSkin;

	// Pistols
	CGroupBox PistolGroup;
	CComboBox GLOCKSkin;
	CComboBox USPSSkin;
	CComboBox DEAGLESkin;
	CComboBox MAGNUMSkin;
	CComboBox DUALSSkin;
	CComboBox FIVESEVENSkin;
	CComboBox TECNINESkin;
	CComboBox P2000Skin;
	CComboBox P250Skin;
	CComboBox CZSkin;

	// MPs


	// Rifles
	CGroupBox Riflegroup;
	CComboBox M41SSkin;
	CComboBox M4A4Skin;
	CComboBox AK47Skin;
	CComboBox AUGSkin;
	CComboBox FAMASSkin;
	CComboBox GALILSkin;
	CComboBox SG553Skin;


	// Machineguns
	CGroupBox MachinegunsGroup;
	CComboBox NEGEVSkin;
	CComboBox M249Skin;

	CGroupBox MPGroup;
	CComboBox MAC10Skin;
	CComboBox P90Skin;
	CComboBox UMP45Skin;
	CComboBox BIZONSkin;
	CComboBox MP7Skin;
	CComboBox MP9Skin;

	// Snipers
	CGroupBox Snipergroup;
	CComboBox SCAR20Skin;
	CComboBox G3SG1Skin;
	CComboBox SSG08Skin;
	CComboBox r8skin;
	CComboBox AWPSkin;

	// Shotguns
	CGroupBox Shotgungroup;
	CComboBox MAG7Skin;
	CComboBox XM1014Skin;
	CComboBox SAWEDOFFSkin;
	CComboBox NOVASkin;

	// Skinsettings
	CGroupBox SkinsettingsGroup;
	CCheckBox StatTrakEnable;
	/*
	*/

};
class aristoispejawindow : public CWindow
{
public:
	void Setup();

	CRageBotTab RageBotTab;
	CLegitBotTab LegitBotTab;
	CVisualTab VisualsTab;
	CMiscTab MiscTab;
	CSettingsTab SettingsTab;
	CColorTab ColorsTab;
	SkinsTab Skins;

	CButton LoadButton;
	CButton SaveButton2;
	CButton LoadButton2;
	CButton SaveButton3;
	CButton LoadButton3;
	CButton UnloadButton;
	CButton PanicButton;
	CButton ThirdButton;
	CButton ByPass;
	CButton FirstButton;
	CComboBox ConfigBox;
};

namespace Menu
{
	void SetupMenu();
	void DoUIFrame();

	extern aristoispejawindow Window;
};