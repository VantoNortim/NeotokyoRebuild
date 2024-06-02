#ifndef NEO_PLAYER_H
#define NEO_PLAYER_H
#ifdef _WIN32
#pragma once
#endif

class CNEO_Player;
class INEOPlayerAnimState;

#include "basemultiplayerplayer.h"
#include "simtimer.h"
#include "soundenvelope.h"
#include "utldict.h"
#include "hl2mp_player.h"
#include "in_buttons.h"

#include "neo_player_shared.h"

class CNEO_Player : public CHL2MP_Player
{
public:
	DECLARE_CLASS(CNEO_Player, CHL2MP_Player);

	CNEO_Player();
	virtual ~CNEO_Player(void);

	static CNEO_Player *CreatePlayer(const char *className, edict_t *ed)
	{
		CNEO_Player::s_PlayerEdict = ed;
		return (CNEO_Player*)CreateEntityByName(className);
	}

	void SendTestMessage(const char *message);
	void SetTestMessageVisible(bool visible);

	DECLARE_SERVERCLASS();
	DECLARE_DATADESC();

	virtual void Precache(void) OVERRIDE;
	virtual void Spawn(void) OVERRIDE;
	virtual void PostThink(void) OVERRIDE;
	virtual void PreThink(void) OVERRIDE;
	virtual void PlayerDeathThink(void) OVERRIDE;
	virtual void SetAnimation(PLAYER_ANIM playerAnim) OVERRIDE;
	virtual bool HandleCommand_JoinTeam(int team) OVERRIDE;
	virtual bool ClientCommand(const CCommand &args) OVERRIDE;
	virtual void CreateViewModel(int viewmodelindex = 0) OVERRIDE;
	virtual bool BecomeRagdollOnClient(const Vector &force) OVERRIDE;
	virtual void Event_Killed(const CTakeDamageInfo &info) OVERRIDE;
	virtual float GetReceivedDamageScale(CBaseEntity* pAttacker) OVERRIDE;
	virtual bool WantsLagCompensationOnEntity(const CBasePlayer *pPlayer, const CUserCmd *pCmd, const CBitVec<MAX_EDICTS> *pEntityTransmitBits) const OVERRIDE;
	virtual void FireBullets(const FireBulletsInfo_t &info) OVERRIDE;
	virtual void Weapon_Equip(CBaseCombatWeapon* pWeapon) OVERRIDE;
	virtual bool Weapon_Switch(CBaseCombatWeapon *pWeapon, int viewmodelindex = 0) OVERRIDE;
	virtual bool Weapon_CanSwitchTo(CBaseCombatWeapon *pWeapon) OVERRIDE;
	virtual bool BumpWeapon(CBaseCombatWeapon *pWeapon) OVERRIDE;
	virtual void ChangeTeam(int iTeam) OVERRIDE;
	virtual void PickupObject(CBaseEntity *pObject, bool bLimitMassAndSize) OVERRIDE;
	virtual void PlayStepSound(Vector &vecOrigin, surfacedata_t *psurface, float fvol, bool force) OVERRIDE;
	virtual void Weapon_Drop(CBaseCombatWeapon *pWeapon, const Vector *pvecTarget = NULL, const Vector *pVelocity = NULL) OVERRIDE;
	virtual void UpdateOnRemove(void) OVERRIDE;
	virtual void DeathSound(const CTakeDamageInfo &info) OVERRIDE;
	virtual CBaseEntity* EntSelectSpawnPoint(void) OVERRIDE;
	virtual void EquipSuit(bool bPlayEffects = true) OVERRIDE;
	virtual void RemoveSuit(void) OVERRIDE;
	virtual void GiveDefaultItems(void) OVERRIDE;
	virtual int	OnTakeDamage_Alive(const CTakeDamageInfo& info) OVERRIDE;

	virtual void InitVCollision(const Vector& vecAbsOrigin, const Vector& vecAbsVelocity) OVERRIDE;

	virtual void ModifyFireBulletsDamage(CTakeDamageInfo* dmgInfo) OVERRIDE;

	virtual const Vector GetPlayerMins(void) const OVERRIDE;
	virtual const Vector GetPlayerMaxs(void) const OVERRIDE;

	// Implementing in header in hopes of compiler picking up the inlined base method
	virtual float GetModelScale() const OVERRIDE
	{
		switch (GetClass())
		{
		case NEO_CLASS_RECON:
			return CBaseAnimating::GetModelScale() * NEO_RECON_MODEL_SCALE;
		case NEO_CLASS_SUPPORT:
			return CBaseAnimating::GetModelScale() * NEO_SUPPORT_MODEL_SCALE;
		default:
			return CBaseAnimating::GetModelScale() * NEO_ASSAULT_MODEL_SCALE;
		}
	}

	void AddNeoFlag(int flags)
	{
		m_NeoFlags.GetForModify() = (GetNeoFlags() | flags);
	}

	void RemoveNeoFlag(int flags)
	{
		m_NeoFlags.GetForModify() = (GetNeoFlags() & ~flags);
	}

	int GetNeoFlags() const { return m_NeoFlags.Get(); }

	void GiveLoadoutWeapon(void);
	void SetPlayerTeamModel(void);
	void SpawnDeadModel(const CTakeDamageInfo& info);
	void SetPlayerCorpseModel(int type);
	virtual void PickDefaultSpawnTeam(void) OVERRIDE;

	virtual bool StartObserverMode(int mode) OVERRIDE;
	virtual void StopObserverMode(void) OVERRIDE;

	virtual bool	CanHearAndReadChatFrom(CBasePlayer *pPlayer) OVERRIDE;

	bool IsCarryingGhost(void) const;

	void ZeroFriendlyPlayerLocArray(void);

	void UpdateNetworkedFriendlyLocations(void);

	void SetDefaultFOV(const int fov);

	void Weapon_AimToggle(CBaseCombatWeapon *pWep, const NeoWeponAimToggleE toggleType);
	void Weapon_AimToggle(CNEOBaseCombatWeapon* pWep, const NeoWeponAimToggleE toggleType);

	// "neo_name" if available otherwise "name"
	// Set "viewFrom" if fetching the name in the view of another player
	const char *GetNeoPlayerName(const CNEO_Player *viewFrom = nullptr) const;
	// "neo_name" even if it's nothing
	const char *GetNeoPlayerNameDirect() const;
	void SetNeoPlayerName(const char *newNeoName);
	void SetClientWantNeoName(const bool b);

	void Lean(void);
	void SoftSuicide(void);
	void GiveAllItems(void);
	bool ProcessTeamSwitchRequest(int iTeam);

	void Weapon_SetZoom(const bool bZoomIn);

	void SuperJump(void);

	void RequestSetClass(int newClass);
	void RequestSetSkin(int newSkin);
	bool RequestSetLoadout(int loadoutNumber);
	void RequestSetStar(int newStar);

	int GetSkin() const { return m_iNeoSkin; }
	int GetClass() const { return m_iNeoClass; }
	int GetStar() const { return m_iNeoStar; }
	bool IsInAim() const { return m_bInAim; }

	bool IsAirborne() const { return (!(GetFlags() & FL_ONGROUND)); }

	virtual void StartAutoSprint(void) OVERRIDE;
	virtual void StartSprinting(void) OVERRIDE;
	virtual void StopSprinting(void) OVERRIDE;
	virtual void InitSprinting(void) OVERRIDE;
	virtual bool CanSprint(void) OVERRIDE;
	virtual void EnableSprint(bool bEnable) OVERRIDE;

	virtual void StartWalking(void) OVERRIDE;
	virtual void StopWalking(void) OVERRIDE;

	// Cloak Power Interface
	void CloakPower_Update(void);
	bool CloakPower_Drain(float flPower);
	void CloakPower_Charge(float flPower);
	float CloakPower_Cap() const;

	float GetNormSpeed_WithActiveWepEncumberment(void) const;
	float GetCrouchSpeed_WithActiveWepEncumberment(void) const;
	float GetWalkSpeed_WithActiveWepEncumberment(void) const;
	float GetSprintSpeed_WithActiveWepEncumberment(void) const;
	float GetNormSpeed_WithWepEncumberment(CNEOBaseCombatWeapon* pNeoWep) const;
	float GetCrouchSpeed_WithWepEncumberment(CNEOBaseCombatWeapon* pNeoWep) const;
	float GetWalkSpeed_WithWepEncumberment(CNEOBaseCombatWeapon* pNeoWep) const;
	float GetSprintSpeed_WithWepEncumberment(CNEOBaseCombatWeapon* pNeoWep) const;
	float GetNormSpeed(void) const;
	float GetCrouchSpeed(void) const;
	float GetWalkSpeed(void) const;
	float GetSprintSpeed(void) const;
	
	int ShouldTransmit( const CCheckTransmitInfo *pInfo) OVERRIDE;

	float GetAttackersScores(const int attackerIdx) const;
	int GetAttackerHits(const int attackerIdx) const;

	void SetNameDupePos(const int dupePos);
	int NameDupePos() const;

	AttackersTotals GetAttackersTotals() const;
	void StartShowDmgStats(const CTakeDamageInfo *info);

	IMPLEMENT_NETWORK_VAR_FOR_DERIVED(m_EyeAngleOffset);

private:
	float GetActiveWeaponSpeedScale() const;
	float GetBackwardsMovementPenaltyScale() const { return ((m_nButtons & IN_BACK) ? NEO_SLOW_MODIFIER : 1.0); }

private:
	void CheckThermOpticButtons();
	void CheckVisionButtons();
	void CheckLeanButtons();
	void PlayCloakSound();
	void CloakFlash();

	bool IsAllowedToSuperJump(void);

	void ShowDmgInfo(char *infoStr, int infoStrSize);
	int SetDmgListStr(char *infoStr, const int infoStrMax, const int playerIdxStart,
		int *infoStrSize, bool *showMenu,
		const CTakeDamageInfo *info) const;

public:
	CNetworkVar(int, m_iNeoClass);
	CNetworkVar(int, m_iNeoSkin);
	CNetworkVar(int, m_iNeoStar);

	CNetworkVar(int, m_iCapTeam);

	CNetworkVar(int, m_iXP);

	CNetworkVar(int, m_iLoadoutWepChoice);
	CNetworkVar(int, m_iNextSpawnClassChoice);

	CNetworkVar(bool, m_bShowTestMessage);
	CNetworkString(m_pszTestMessage, 32 * 2 + 1);

	CNetworkVector(m_vecGhostMarkerPos);
	CNetworkVar(bool, m_bGhostExists);
	CNetworkVar(bool, m_bInThermOpticCamo);
	CNetworkVar(bool, m_bLastTickInThermOpticCamo);
	CNetworkVar(bool, m_bInVision);
	CNetworkVar(bool, m_bHasBeenAirborneForTooLongToSuperJump);
	CNetworkVar(bool, m_bInAim);
	CNetworkVar(int, m_bInLean);
	CNetworkVar(bool, m_bDroppedAnything);

	CNetworkVar(float, m_flCamoAuxLastTime);
	CNetworkVar(int, m_nVisionLastTick);

	CNetworkArray(Vector, m_rvFriendlyPlayerPositions, MAX_PLAYERS);
	CNetworkArray(float, m_rfAttackersScores, (MAX_PLAYERS + 1));
	CNetworkArray(int, m_rfAttackersHits, (MAX_PLAYERS + 1));

	CNetworkVar(unsigned char, m_NeoFlags);
	CNetworkString(m_szNeoName, MAX_PLAYER_NAME_LENGTH);
	CNetworkVar(int, m_szNameDupePos);

	// NEO NOTE (nullsystem): As dumb as client sets -> server -> client it may sound,
	// cl_fakenick directly doesn't even work properly for client set convars anyway
	CNetworkVar(bool, m_bClientWantNeoName);

	bool m_bIsPendingSpawnForThisRound;

private:
	bool m_bFirstDeathTick;
	bool m_bCorpseSpawned;
	bool m_bPreviouslyReloading;
	bool m_szNeoNameHasSet;

	float m_flLastAirborneJumpOkTime;
	float m_flLastSuperJumpTime;

	// Non-network version of m_szNeoName with dupe checker index
	mutable char m_szNeoNameWDupeIdx[MAX_PLAYER_NAME_LENGTH + 10];
	mutable bool m_szNeoNameWDupeIdxNeedUpdate;

	int m_iDmgMenuCurPage;
	int m_iDmgMenuNextPage;

	INEOPlayerAnimState* m_pPlayerAnimState;

private:
	CNEO_Player(const CNEO_Player&);
};

inline CNEO_Player *ToNEOPlayer(CBaseEntity *pEntity)
{
	if (!pEntity || !pEntity->IsPlayer())
	{
		return NULL;
	}
#if _DEBUG
	Assert(dynamic_cast<CNEO_Player*>(pEntity));
#endif
	return static_cast<CNEO_Player*>(pEntity);
}

#endif // NEO_PLAYER_H
