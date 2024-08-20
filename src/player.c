#include "player.h"

Error initPlayer(Player *player, const char *player_name, SDL_Renderer *renderer) {
	printf("initializing player\n\n");
	int error;

	Player player_data = {
		.x = 0, .y = 0, .xvel = 0, .yvel = 0, .dir = 1,
		.animation = IDLE, .frame = 0,
		.animation_frame_counter = 0,
		.attached_to_gamepad = 0,
	};
	*player = player_data;

	if ((error = initPlayerConstants(player, player_name))) return error;

	printf("initializing player animations\n");
	if ((error = initPlayerAnimationSheets(&player->animation_sheets, player_name, renderer))) return error;
	if ((error = initPlayerAnimationData(&player->animation_data, player_name))) return error;

	return SUCCESS;
}

Error initPlayerConstants(Player *player, const char *player_name) {
	printf("initializing player constants\n");
	if (!strcmp(player_name, "turtle")) {
		player->xaccel = TURTLE_XACCEL;
		player->yaccel = TURTLE_YACCEL;
		player->max_xvel = TURTLE_MAX_XVEL;
		player->max_yvel = TURTLE_MAX_YVEL;
	}
	return SUCCESS;
}

int initPlayerAnimationSheets(
		SDL_Texture ** *animation_sheets,
		const char *player_name,
		SDL_Renderer *renderer
) {
	printf("loading player animation files\n");
	*animation_sheets = (SDL_Texture **) malloc(getPlayerNumAnimations(player_name) * sizeof(SDL_Texture *));

	char player_directory[20] = "";
	strcpy(player_directory, "img/");
	strcat(player_directory, player_name);
	strcat(player_directory, "/");

	struct dirent *dirent_object;
	DIR *directory = opendir(player_directory);
	if (!directory) {
		fprintf(stderr, "Couldn't open directory %s\n", player_directory);
		return FILESYSTEM_ERROR;
	}

	printf("searching for animation files\n");
	char filename[40] = "";
	int animation_index;
	SDL_Texture *texture;

	while ((dirent_object = readdir(directory)) != NULL) {
		if (dirent_object->d_name[0] != '.')
			printf("found file \"%s\"\n", dirent_object->d_name);

		if ((animation_index = getPlayerAnimationIndex(dirent_object->d_name)) == -1)
			continue;
		strcpy(filename, player_directory);
		strcat(filename, dirent_object->d_name);

		texture = IMG_LoadTexture(renderer, filename);
		if (texture == NULL)
			return SDL_ERROR;
		*animation_sheets[animation_index] = texture;
	}
	return SUCCESS;
}

int getPlayerNumAnimations(const char *player_name) {
	if (!strcmp(player_name, "turtle")) return NUM_TURTLE_ANIMATIONS;
	return -1;
}

int getPlayerAnimationIndex(const char *animation_name) {
	if (!strcmp(animation_name, "idle.png")) return IDLE;

	return -1;
}

Error initPlayerAnimationData(int8_t ** *animation_data, const char *player_name) {
	printf("loading player animation data\n\n");
	if (!strcmp(player_name, "turtle")) *animation_data = TURTLE_ANIMATION_DATA;
	return SUCCESS;
}

Error updatePlayer(Player *player) {
	//printf("updating player\n");

	player->animation_frame_counter++;
	if (player->animation_frame_counter >= PLAYER_MOVEMENT_FRAMERATE) player->animation_frame_counter = 0;
	player->frame += !player->animation_frame_counter;
	player->frame *= player->animation_data[player->animation][player->frame] != -1;

	player->movement_frame_counter++;
	if (player->movement_frame_counter >= PLAYER_MOVEMENT_FRAMERATE) player->movement_frame_counter = 0;

	if (!player->movement_frame_counter && !player->freeze_timer) {
		if (!player->control_status.dodge) {
			player->xvel += player->control_status.movement_horizontal * player->xaccel;
			if (abs(player->xvel) > player->max_xvel) player->xvel = player->max_xvel * player->dir;
		}
	}

	player->x += player->xvel;
	player->y += player->yvel;
	if (player->xvel) player->dir = player->xvel > 0 ? 1 : -1;

	return SUCCESS;
}

Error drawPlayer(Player player, SDL_Renderer *renderer) {
	const SDL_Rect player_rect = {
		.x = player.x, .y = player.y,
		.w = PLAYER_WIDTH, .h = PLAYER_HEIGHT
	};
	const SDL_Rect clip_rect = {
		.x = player.animation_data[player.animation][player.frame] * PLAYER_TEXTURE_WIDTH, .y = 0,
		.w = PLAYER_TEXTURE_WIDTH, .h = PLAYER_TEXTURE_HEIGHT
	};
	SDL_Texture *texture = player.animation_sheets[player.animation];
	if (SDL_RenderCopy(renderer, texture, &clip_rect, &player_rect) < 0)
		return SDL_ERROR;

	return SUCCESS;
}

void freePlayer(Player *player) {
	printf("freeing player\n");
	for (int animation_index = 0; animation_index < ANIMATION_COUNT; animation_index++) {
		printf("freeing animation %d\n", animation_index);
		SDL_DestroyTexture(player->animation_sheets[animation_index]);
	}
	free(player->animation_sheets);
	printf("\n");
}
