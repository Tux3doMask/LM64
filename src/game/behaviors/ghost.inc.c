// ghost.inc.c

static struct ObjectHitbox sGHostHitbox = {
    /* interactType:      */ INTERACT_MR_BLIZZARD,
    /* downOffset:        */ 0,
    /* damageOrCoinValue: */ 3,
    /* health:            */ 1,
    /* numLootCoins:      */ 0,
    /* radius:            */ 80,
    /* height:            */ 160,
    /* hurtboxRadius:     */ 80,
    /* hurtboxHeight:     */ 160,
};

void ghost_chase(void) {
	cur_obj_rotate_yaw_toward(o->oAngleToMario, 0x400);
	o->oForwardVel = 10.0f;
	
	if (o->oDistanceToMario < 10.0f) {
		o->oAction = GHOST_CHASE_CLOSE;
	}
}

void ghost_running(void) {
	cur_obj_rotate_yaw_toward(o->oAngleToMario + 0x8000, 0x400);
	if (o->oTimer >= 300) {
		o->oPoltergustStatus = GHOST_DESPAWN;
		o->oTimer = 0;
	}
}

void ghost_chase_close(void) {
	o->oForwardVel = 2.0f;
	if (o->oDistanceToMario < 2.0f) {
		o->oAction = GHOST_PUNCH;
		o->oTimer = 0;
	}
}

void ghost_caught(void) {
	if (o->oTimer >= 30) {
		o->oAction = GHOST_RUNNING;
		o->oTimer = 0;
		o->oForwardVel = random_float() * 2;
	}
}

void ghost_punch(void) {
	o->oForwardVel = 0.0f;
	
}

void ghost_spawn(void) {
	o->oAction = GHOST_CHASE;
}

void ghost_reset(void) {
	vec3f_copy(&o->oPosVec, &o->oHomeVec);
	if (o->oPoltergustStatus & POLTERGUST_GHOST_IN_FRONT) {
		o->oTimer = 0;
	}
	if (o->oTimer >= 30) {
		o->oAction = GHOST_SPAWN;
	}
}

void ghost_despawn(void) {
	if (o->oTimer > 10) {
		o->oAction = GHOST_RESET;
	}
}

void update_gold_ghost(void) {
	cur_obj_update_floor_and_walls();
	
	switch (o->oAction) {
		case GHOST_CHASE:		ghost_chase();			break;
		case GHOST_RUNNING:		ghost_running();		break;
		case GHOST_CHASE_CLOSE: ghost_chase_close();	break;
		case GHOST_PUNCH:		ghost_punch();			break;
		case GHOST_CAUGHT:		ghost_caught();			break;
		case GHOST_SPAWN:       ghost_spawn();			break;
		case GHOST_DESPAWN:		ghost_despawn();		break;
		case GHOST_RESET:		ghost_reset();			break;
	}
	
	if (!(o->oPoltergustStatus & POLTERGUST_GHOST_LATCHED) &&
		o->oPoltergustStatus & POLTERGUST_GHOST_FLASHED &&
		o->oPoltergustStatus & POLTERGUST_IN_STREAM
		) {
		o->oPoltergustStatus |= POLTERGUST_GHOST_LATCHED;
		o->oAction = GHOST_CAUGHT;
		o->oTimer = 0;
	}
	
	cur_obj_move_standard(-78);
}