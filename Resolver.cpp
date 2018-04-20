#include "Resolver.h"
#include "Ragebot.h"
#include "Hooks.h"
#include "LagComp.h"
int IClientEntity::GetSequenceActivity(int sequence)
{
	auto hdr = Interfaces::ModelInfo->GetStudiomodel(this->GetModel());

	if (!hdr)
		return -1;

	static auto getSequenceActivity = (DWORD)(Utilities::Memory::FindPatternV2("client.dll", "55 8B EC 83 7D 08 FF 56 8B F1 74"));
	static auto GetSequenceActivity = reinterpret_cast<int(__fastcall*)(void*, studiohdr_t*, int)>(getSequenceActivity);

	return GetSequenceActivity(this, hdr, sequence);
}
/*Animation List*/
/*
activity == 961 && flweight == 1.f && flcycle <= 0.0f --- Reloading/shooting
activity == 979 && flweight == 1.0f && flcycle == 0.f --- lby update?
*/
BackTrack* lbybacktrack;
bool lowerBodyYawUpdated(IClientEntity* pEntity) {
	for (int w = 0; w < 13; w++)
	{
		AnimationLayer currentLayer = pEntity->GetAnimOverlay(w);
		const int activity = pEntity->GetSequenceActivity(currentLayer.m_nSequence);
		float flcycle = currentLayer.m_flCycle, flprevcycle = currentLayer.m_flPrevCycle, flweight = currentLayer.m_flWeight, flweightdatarate = currentLayer.m_flWeightDeltaRate;
		uint32_t norder = currentLayer.m_nOrder;
		if (activity == 973 && flweight == 1.f || activity == 979 && flweight == 1.0f && currentLayer.m_flPrevCycle != currentLayer.m_flCycle)// 961 looks like they shot //flweight seems like right as the animation or right after 972 could be moving
			return true;
	}
	return false;
}
//gonna fix resolver for you. with correct animation statements from vids i watched and other resolvers that showed the same results.
bool IsBreakingLBY120(IClientEntity* pEntity)
{
	for (int w = 0; w < 13; w++)
	{
		AnimationLayer prevlayer;
		AnimationLayer currentLayer = pEntity->GetAnimOverlay(w);
		const int activity = pEntity->GetSequenceActivity(currentLayer.m_nSequence);
		float flcycle = currentLayer.m_flCycle, flprevcycle = currentLayer.m_flPrevCycle, flweight = currentLayer.m_flWeight, flweightdatarate = currentLayer.m_flWeightDeltaRate;
		uint32_t norder = currentLayer.m_nOrder;
		if (activity == ACT_CSGO_IDLE_ADJUST_STOPPEDMOVING || activity == ACT_CSGO_IDLE_TURN_BALANCEADJUST && flweight >= .99 && currentLayer.m_flPrevCycle != currentLayer.m_flCycle)
		{
			float flanimTime = currentLayer.m_flCycle, flsimtime = pEntity->GetSimulationTime();

			return true;//force lby-180?
		}
		prevlayer = currentLayer;
		return false;
	}
	return false;
}
bool IsResolvableByLBY(IClientEntity* pEntity)
{
	for (int w = 0; w < 13; w++)
	{
		AnimationLayer prevlayer;
		AnimationLayer currentLayer = pEntity->GetAnimOverlay(w);
		const int activity = pEntity->GetSequenceActivity(currentLayer.m_nSequence);
		float flcycle = currentLayer.m_flCycle, flprevcycle = currentLayer.m_flPrevCycle, flweight = currentLayer.m_flWeight, flweightdatarate = currentLayer.m_flWeightDeltaRate;
		uint32_t norder = currentLayer.m_nOrder;
		if (activity == 979 && currentLayer.m_flWeight == 0.f && currentLayer.m_flPrevCycle != currentLayer.m_flCycle)
		{
			return true;//bruteeee because breaking lby <120 
		}
		prevlayer = currentLayer;
	}
	return false;
}
bool playerStoppedMoving(IClientEntity* pEntity)
{
	for (int w = 0; w < 13; w++)
	{
		AnimationLayer currentLayer = pEntity->GetAnimOverlay(w);
		const int activity = pEntity->GetSequenceActivity(currentLayer.m_nSequence);
		float flcycle = currentLayer.m_flCycle, flprevcycle = currentLayer.m_flPrevCycle, flweight = currentLayer.m_flWeight, flweightdatarate = currentLayer.m_flWeightDeltaRate;
		uint32_t norder = currentLayer.m_nOrder;
		if (activity == ACT_CSGO_IDLE_ADJUST_STOPPEDMOVING)
			return true;
	}
	return false;
}

bool lowerBodyPrediction(IClientEntity* pEntity)
{
	static float prediction = 0.f;
	static bool secondrun = false;
	float flServerTime = (float)pEntity->GetTickBase() * Interfaces::Globals->interval_per_tick;
	if (playerStoppedMoving(pEntity) && !secondrun)
	{
		prediction = flServerTime + 0.22;
		secondrun = true;
	}
	else if (pEntity->GetVelocity().Length2D() < 0.1f && secondrun && prediction <= pEntity->GetSimulationTime())
	{
		prediction = pEntity->GetTickBase() + 1.1f;
	}
	else//theyre moving
	{
		secondrun = false;
		return false;
	}
	if (prediction <= pEntity->GetSimulationTime())
	{
		return true;
	}
	return false;
}

bool isflying(IClientEntity* pEntity)
{
	if (!pEntity->GetFlags() & FL_ONGROUND)
	{
		return true;
	}
	return false;
}
void ResolverSetup::Pitches(IClientEntity* pEntity)
{

}



void ResolverSetup::LowerBody(IClientEntity* pEntity)
{
	static float playerYaw[64];
	static float playerMovingLby[64];
	static float updatedYaw[64];
	auto index = pEntity->GetIndex();
	auto velocity = pEntity->GetVelocity().Length2D();
	playerYaw[index] = pEntity->GetEyeAnglesXY()->y;

	if (Menu::Window.RageBotTab.AimbotResolver.GetIndex())
	{
		if (velocity > 0.1f && !isflying(pEntity))//fakewalk check meme
		{
			playerYaw[index] = *pEntity->GetLowerBodyYawTarget();
			playerMovingLby[index] = *pEntity->GetLowerBodyYawTarget();
		}

		else if (IsResolvableByLBY(pEntity) && Globals::missedshots < 2 && !isflying(pEntity) && !IsBreakingLBY120(pEntity) && !lowerBodyPrediction(pEntity))//breaking lby <120 or 
		{
			playerYaw[index] = *pEntity->GetLowerBodyYawTarget();
		}
		else if (IsBreakingLBY120(pEntity) && Globals::missedshots < 2 && !lowerBodyPrediction(pEntity) && !isflying(pEntity))//balance adjust was triggered causing 979
		{
			/*150 is avg*/

			playerYaw[index] = updatedYaw[index] + 150;//this also means we could force lby upon that single tick...
		}
		else if (lowerBodyPrediction(pEntity))//tbh we prob want to flick it 180 here
		{
			playerYaw[index] = *pEntity->GetLowerBodyYawTarget();
			updatedYaw[index] = *pEntity->GetLowerBodyYawTarget();

			lbyupdated = true;
		}
		if (Globals::missedshots > 2 || isflying(pEntity))
		{
			playerYaw[index] = *pEntity->GetLowerBodyYawTarget() + 60 * Globals::missedshots;
		}

		else
		{
			playerYaw[index] = playerMovingLby[index];
		}
	}
	pEntity->GetEyeAnglesXY()->y = playerYaw[index];
}


void ResolverSetup::CM(IClientEntity* pEntity)
{
	for (int x = 1; x < Interfaces::Engine->GetMaxClients(); x++)
	{
		pEntity = (IClientEntity*)Interfaces::EntList->GetClientEntity(x);

		if (!pEntity
			|| pEntity == hackManager.pLocal()
			|| pEntity->IsDormant()
			|| !pEntity->IsAlive())
			continue;

		//ResolverSetup::StoreThings(pEntity);
	}
}

void ResolverSetup::FSN(IClientEntity* pEntity, ClientFrameStage_t stage)
{
	for (int i = 1; i < Interfaces::Engine->GetMaxClients(); i++)
	{
		pEntity = (IClientEntity*)Interfaces::EntList->GetClientEntity(i);
		if (!pEntity || pEntity == hackManager.pLocal() || pEntity->IsDormant() || !pEntity->IsAlive())
			continue;
		ResolverSetup::LowerBody(pEntity);
		//ResolverSetup::Pitches(pEntity);

	}
}