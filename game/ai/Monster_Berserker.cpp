
#include "../../idlib/precompiled.h"
#pragma hdrstop

#include "../Game_local.h"

extern const char* aiActionStatusString [ rvAIAction::STATUS_MAX ];

class rvMonsterBerserker : public idAI {
public:

	CLASS_PROTOTYPE( rvMonsterBerserker );

	rvMonsterBerserker ( void );

	void				Spawn							( void );
	void				Save							( idSaveGame *savefile ) const;
	void				Restore							( idRestoreGame *savefile );

	// Add some dynamic externals for debugging
	virtual void		GetDebugInfo					( debugInfoProc_t proc, void* userData );

	virtual bool		Pain							( idEntity *inflictor, idEntity *attacker, int damage, const idVec3 &dir, int location );
protected:

	virtual bool		CheckPainActions				( void );
	virtual bool		CheckActions					( void );
	int					FilterTactical					( int availableTactical );
	virtual void		OnTacticalChange				( aiTactical_t oldTactical );

private:

	int					standingMeleeNoAttackTime;
	int					painConsecutive;

	// Actions
	rvAIAction			actionPopupAttack;
	rvAIAction			actionChargeAttack;

	bool				Berz_CanHitEnemyFromAnim		( int animNum );
	bool				CheckAction_RangedAttack		( rvAIAction* action, int animNum );
	bool				CheckAction_ChargeAttack		( rvAIAction* action, int animNum );

	// Global States
	stateResult_t		State_Killed					( const stateParms_t& parms );

	// Torso States
	stateResult_t		State_Torso_Pain				( const stateParms_t& parms );
	stateResult_t		State_Torso_ChargeAttack		( const stateParms_t& parms );

	// Frame commands
	stateResult_t		Frame_ChargeGroundImpact		( const stateParms_t& parms );
	stateResult_t		Frame_DoBlastAttack				( const stateParms_t& parms );

	CLASS_STATES_PROTOTYPE ( rvMonsterBerserker );
};

CLASS_DECLARATION( idAI, rvMonsterBerserker )
END_CLASS

/*
================
rvMonsterBerserker::rvMonsterBerserker
================
*/
rvMonsterBerserker::rvMonsterBerserker ( ) {
	painConsecutive	= 0;
	standingMeleeNoAttackTime = 0;
}

/*
================
rvMonsterBerserker::Spawn
================
*/
void rvMonsterBerserker::Spawn ( void ) {
	actionPopupAttack.Init  ( spawnArgs, "action_popupAttack",	NULL,					AIACTIONF_ATTACK );
	actionChargeAttack.Init ( spawnArgs, "action_chargeAttack", "Torso_ChargeAttack",	AIACTIONF_ATTACK );
	PlayEffect( "fx_ambient_electricity", animator.GetJointHandle( "r_Lowerarm_Real" ), true );
	PlayEffect( "fx_ambient_electricity_mace", animator.GetJointHandle( "chain9" ), true );
}

/*
================
rvMonsterBerserker::Save
================
*/
void rvMonsterBerserker::Save ( idSaveGame *savefile ) const {
	actionPopupAttack.Save ( savefile );
	actionChargeAttack.Save ( savefile );
	savefile->WriteInt( painConsecutive );
	savefile->WriteInt( standingMeleeNoAttackTime );
}

/*
================
rvMonsterBerserker::Restore
================
*/
void rvMonsterBerserker::Restore ( idRestoreGame *savefile ) {
	actionPopupAttack.Restore ( savefile );
	actionChargeAttack.Restore ( savefile );
	savefile->ReadInt( painConsecutive );
	savefile->ReadInt( standingMeleeNoAttackTime );
}

/*
================
rvMonsterBerserker::CheckAction_ChargeAttack
================
*/
bool rvMonsterBerserker::CheckAction_ChargeAttack ( rvAIAction* action, int animNum ) {
	return false;
}

/*
============
rvMonsterBerserker::CanHitEnemyFromAnim
============
*/
bool rvMonsterBerserker::Berz_CanHitEnemyFromAnim( int animNum ) {
	return false;
}

/*
================
rvMonsterBerserker::CheckAction_RangedAttack
================
*/
bool rvMonsterBerserker::CheckAction_RangedAttack ( rvAIAction* action, int animNum ) {
	return false;
}

/*
================
rvMonsterBerserker::CheckActions
================
*/
bool rvMonsterBerserker::CheckActions(void) {
	return false;
}

/*
================
rvMonsterBerserker::FilterTactical
================
*/
int rvMonsterBerserker::FilterTactical ( int availableTactical ) {
	if ( move.moveCommand == MOVE_TO_ENEMY && move.moveStatus == MOVE_STATUS_DEST_UNREACHABLE ) {
		availableTactical |= AITACTICAL_RANGED_BIT;
	} else if ( combat.tacticalCurrent != AITACTICAL_RANGED 
		&& combat.tacticalCurrent != AITACTICAL_MELEE 
		&& (combat.tacticalMaskAvailable&AITACTICAL_RANGED_BIT) ) {
		availableTactical |= AITACTICAL_RANGED_BIT;
	} else {
		availableTactical &= ~AITACTICAL_RANGED_BIT;
	}
	
	return idAI::FilterTactical ( availableTactical );
}

/*
================
rvMonsterBerserker::OnTacticalChange

Enable/Disable the ranged attack based on whether the berzerker needs it
================
*/
void rvMonsterBerserker::OnTacticalChange ( aiTactical_t oldTactical ) {
	switch ( combat.tacticalCurrent ) {
		case AITACTICAL_MELEE:
			actionRangedAttack.fl.disabled = true;
			//once you've gone into melee once, it's okay to try ranged attacks later
			combat.tacticalMaskAvailable |= AITACTICAL_RANGED_BIT;
			break;

		default:
			actionRangedAttack.fl.disabled = false;
			break;
	}
}

/*
=====================
rvMonsterBerserker::GetDebugInfo
=====================
*/
void rvMonsterBerserker::GetDebugInfo ( debugInfoProc_t proc, void* userData ) {
	// Base class first
	idAI::GetDebugInfo ( proc, userData );
	
	proc ( "idAI", "action_popupAttack",		aiActionStatusString[actionPopupAttack.status], userData );
	proc ( "idAI", "action_chargeAttack",		aiActionStatusString[actionChargeAttack.status], userData );
}

/*
================
rvMonsterBerserker::Pain
================
*/
bool rvMonsterBerserker::Pain( idEntity *inflictor, idEntity *attacker, int damage, const idVec3 &dir, int location ) {
	if ( pain.lastTakenTime > gameLocal.GetTime() - 500 ) {
		painConsecutive++;
	} else {
		painConsecutive = 1;
	}
	return ( idAI::Pain( inflictor, attacker, damage, dir, location ) );
}

/*
================
rvMonsterBerserker::CheckPainActions
================
*/
bool rvMonsterBerserker::CheckPainActions ( void ) {
	if ( !pain.takenThisFrame || !actionTimerPain.IsDone ( actionTime ) ) {
		return false;
	}
	
	if ( !pain.threshold || pain.takenThisFrame < pain.threshold ) {
		if ( painConsecutive < 10 ) {
			return false;
		} else {
			painConsecutive = 0;
		}
	}
	
	PerformAction ( "Torso_Pain", 2, true );
	actionTimerPain.Reset ( actionTime );
	
	return true;	
}

/*
===============================================================================

	States 

===============================================================================
*/

CLASS_STATES_DECLARATION ( rvMonsterBerserker )
	STATE ( "State_Killed",					rvMonsterBerserker::State_Killed )

	STATE ( "Torso_ChargeAttack",			rvMonsterBerserker::State_Torso_ChargeAttack )
	STATE ( "Torso_Pain",					rvMonsterBerserker::State_Torso_Pain )
	
	STATE ( "Frame_ChargeGroundImpact",		rvMonsterBerserker::Frame_ChargeGroundImpact )
	STATE ( "Frame_DoBlastAttack",			rvMonsterBerserker::Frame_DoBlastAttack )
END_CLASS_STATES

/*
================
rvMonsterBerserker::State_Torso_ChargeAttack
================
*/
stateResult_t rvMonsterBerserker::State_Torso_ChargeAttack ( const stateParms_t& parms ) {
	return SRESULT_DONE;
}

/*
================
rvMonsterBerserker::State_Torso_Pain
================
*/
stateResult_t rvMonsterBerserker::State_Torso_Pain ( const stateParms_t& parms ) {
	StopEffect ( "fx_charge_up" );
	return idAI::State_Torso_Pain ( parms );
}

/*
================
rvMonsterBerserker::State_Killed
================
*/
stateResult_t rvMonsterBerserker::State_Killed	( const stateParms_t& parms ) {
	StopEffect ( "fx_charge_up" );
	StopEffect ( "fx_ambient_electricity" );
	StopEffect ( "fx_ambient_electricity_mace" );
	return idAI::State_Killed ( parms );
}

/*
================
rvMonsterBerserker::Frame_ChargeGroundImpact
================
*/
stateResult_t rvMonsterBerserker::Frame_ChargeGroundImpact ( const stateParms_t& parms ) {
	idVec3			start;
	idVec3			end;
	idMat3			axis;
	trace_t			tr;

	GetJointWorldTransform ( animator.GetJointHandle ( "R_lowerArm_Real" ), gameLocal.time, start, axis );
	
	end    = start;
	end.z -= 128;
// RAVEN BEGIN
// ddynerman: multiple clip worlds
	gameLocal.TracePoint ( this, tr, start, end, MASK_SHOT_BOUNDINGBOX, this );
// RAVEN END
	
	gameLocal.PlayEffect ( gameLocal.GetEffect( spawnArgs, "fx_ground_impact" ), tr.endpos, idVec3(0,0,1).ToMat3() ); 
	
	return SRESULT_OK;
}

/*
================
rvMonsterBerserker::Frame_DoBlastAttack
================
*/
stateResult_t rvMonsterBerserker::Frame_DoBlastAttack ( const stateParms_t& parms ) {
	float			i;
	idVec3			start;
	idMat3			axis;
	idAngles		angles ( 0.0f, move.current_yaw, 0.0f );
	const idDict*	blastDict;

	blastDict = gameLocal.FindEntityDefDict ( spawnArgs.GetString ( "def_attack_spike" ), false );
	if ( !blastDict ) {
		gameLocal.Error ( "missing projectile on spike attack for AI entity '%s'", GetName ( ) ) ;
		return SRESULT_OK;
	}

	GetJointWorldTransform ( animator.GetJointHandle ( "end_spike" ), gameLocal.time, start, axis );

	for( i = 0; i < 32; i++ ) {
		angles.yaw += (360.0f / 32.0f);
		AttackProjectile ( blastDict, start, angles );
	}
	
	return SRESULT_OK;
}
