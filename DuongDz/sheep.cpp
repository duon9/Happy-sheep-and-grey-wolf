#include "sheep.h"

SDL_Texture* Sheep::texture = nullptr;

void Sheep::init() {
	//SDL_Log("Sheep init \n");
	setLocation({ Rand::Randint(0, SCREENWIDTH - 100), Rand::Randint(0, SCREENHEIGHT - 80) });
	loadClips();
	des.w = 100;
	des.h = 80;
}

void Sheep::loadClips() {
	//SDL_Log("Sheep load \n");
	for (int i = 0; i < 6; i++) {
		//SDL_Log("Sheep  \n");
		SDL_Rect idle = { 105 * i, 0, 105, 90 };
		idle_clips.push_back(idle);
		SDL_Rect walk = { 105 * i, 90, 105, 90 };
		clips.push_back(walk);
	}
}

void Sheep::render() {
	if (status == IDLE) {
		frameCount++;
		if (frameCount >= MAX_SHEEP_FRAME_IDLE - 1) frameCount = 0;
		src = idle_clips[frameCount / 20];
	}
	else if (status == WALK) {
		frameCount++;
		if (frameCount >= MAX_SHEEP_FRAME_WALK - 1) frameCount = 0;
		src = clips[frameCount / 5];
	}
	SDL_RenderCopyEx(Global::renderer, texture, &src, &des, NULL, NULL, flip);
}

void Sheep::actions(Wolf* wolf) {
	Vector2D v = Vector2D(getPosition(), wolf->getPosition());
	if (trackWolf(v)) {
		status = WALK;
		moveToSafeZone(v);
	}
	else {
		status = IDLE;
	}
}

bool Sheep::trackWolf(Vector2D v) {
	if (v.length() <= 32 * 10) return true;
	return false;
}

void Sheep::moveToSafeZone(Vector2D v) {
	if (v.getX() > 0) flip = SDL_FLIP_HORIZONTAL;
	else if (v.getX() < 0) flip = SDL_FLIP_NONE;
	v.reverse();
	//Vector2D t = v;
	if (/*v.getX() != 0 && v.getY() != 0*/ true) v.normalize();
	if (!isCollideHorizontal(v.getX() * speed)) des.x += v.getX() * speed;
	if (!isCollideVertical(v.getY() * speed)) des.y += v.getY() * speed;
}

bool Sheep::isCollideHorizontal(int vel) {
	if (des.x + vel < 0 || des.x + des.w + vel > SCREENWIDTH) return true;
	return false;
}

bool Sheep::isCollideVertical(int vel) {
	if (des.y + vel < 0 || des.y + des.h + vel > SCREENHEIGHT) return true;
	return false;
}