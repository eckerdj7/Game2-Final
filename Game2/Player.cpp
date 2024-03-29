#include "Player.h"

Player::Player()
{
	speed = 0;
	normalSpeed = 0;
	sprintBoost = 0;
	direction = Vector3(0,0,1);
	position = Vector3(0,0,0);
	dirTheta = 0;
	turnSpeed = 3.5;
	gameTime = 0;
	colliding = false;
	canStrafe = false;
	mousePos = Vector2(0.0f, 0.0f);
	lastMousePos = mousePos;	
	left = Vector3(1,0,0);
	right = Vector3(-1,0,0);
	forward = Vector3(0,0,-1);
	back = Vector3(0,0,1);
	up = Vector3(0,1,0);
	down = Vector3(0,-1,0);
	zero = Vector3(0,0,0);
}

Player::~Player()
{
	delete torso;
	delete head;
	delete rightArm;
	delete leftArm;
	delete rightLeg;
	delete leftLeg;
}

void Player::init(string n, Vector3 pos, float spd, float height, float width, float depth, ID3D10Device* d, Light* light)
{
	device = d;

	//HR(D3DX10CreateShaderResourceViewFromFile(device, 
	//	L"face.jpg", 0, 0, &textures.rv[0], 0 ));
	//HR(D3DX10CreateShaderResourceViewFromFile(device, 
	//	L"side.jpg", 0, 0, &textures.rv[1], 0 ));
	//HR(D3DX10CreateShaderResourceViewFromFile(device, 
	//	L"side.jpg", 0, 0, &textures.rv[2], 0 ));
	//HR(D3DX10CreateShaderResourceViewFromFile(device, 
	//	L"side.jpg", 0, 0, &textures.rv[3], 0 ));
	//HR(D3DX10CreateShaderResourceViewFromFile(device, 
	//	L"headTop.jpg", 0, 0, &textures.rv[4], 0 ));
	//HR(D3DX10CreateShaderResourceViewFromFile(device, 
	//	L"noTex.jpg", 0, 0, &textures.rv[5], 0 ));

	name = n;
	position = pos;
	speed = spd;
	normalSpeed = speed;
	sprintBoost = speed * 3.3f;
	limbSpeed = speed * 0.5f;
	this->height = height;
	this->width = width;
	this->depth = depth;
	//make positions points from width, depth
	perimeter.push_back(Vector3(pos.x - width/2 - 1.4f, 0.1f, pos.z - depth/2 - 1.4f));
	perimeter.push_back(Vector3(pos.x, 0.1f, pos.z - depth/2 - 1.4f));
	perimeter.push_back(Vector3(pos.x + width/2 + 1.4f, 0.1f, pos.z - depth/2 - 1.4f));
	perimeter.push_back(Vector3(pos.x + width/2 + 1.4f, 0.1f, pos.z));
	perimeter.push_back(Vector3(pos.x + width/2 + 1.4f, 0.1f, pos.z + depth/2 + 1.4f));
	perimeter.push_back(Vector3(pos.x, 0.1f, pos.z + depth/2 + 1.4f));
	perimeter.push_back(Vector3(pos.x - width/2 - 1.4f, 0.1f, pos.z + depth/2 + 1.4f));
	perimeter.push_back(Vector3(pos.x - width/2 - 1.4f, 0.1f, pos.z));
	spotLight = light;

	buildBody();
}

void Player::setEffectVariables(ID3D10EffectMatrixVariable* wvpVar, ID3D10EffectMatrixVariable* worldVar)
{
	mfxWVPVar = wvpVar;
	mfxWorldVar = worldVar;
}

void Player::buildBody()
{

	
	
	torso = new BodyPart();
	head = new BodyPart();
	rightArm = new BodyPart();
	leftArm = new BodyPart();
	rightLeg = new BodyPart();
	leftLeg = new BodyPart();
	rightShin = new BodyPart();
	leftShin = new BodyPart();

	// torso
	Vector3 tPos = position;
	tPos.y += height * 0.5f;
	tPos = Vector3(0, 0, 0);
	Box* b = new Box();
	b->init(device, width, height * 0.36f, depth, LightBlue, LightBlue);
	b->setDiffuseMap(diffuseMapVar);
	torso->init("torso", b, tPos, direction, Vector3(width, height * 0.36f, depth), speed);
	torso->setBody(this);

	// head
	b = new Box();
	b->init(device, width * 0.45f, height * 0.12f, depth * 0.9f, LightBlue, LightBlue);
	b->setDiffuseMap(diffuseMapVar);
	Vector3 hPos = position;
	hPos.y += height * 0.38f;
	head->init("head", b, hPos, direction, Vector3(width * 0.45f, height * 0.12f, depth * 0.9f), speed);
	head->setBody(this);
	head->setRoot(torso);

	// right arm
	b = new Box();
	Vector3 raPos = position;
	Vector3 torsoSize = torso->getSize();
	raPos += Vector3(width * 0.65f, torsoSize.y * 0.8f, 0);
	b->init(device, width * 0.2f, height * 0.37f, depth * 0.5f, LightBlue, LightBlue);
	b->setDiffuseMap(diffuseMapVar);
	rightArm->init("rightArm", b, raPos, direction, Vector3(width * 0.2f, height * 0.37f, depth * 0.5f), speed);
	rightArm->setBody(this);
	rightArm->setRoot(torso);

	// left arm
	b = new Box();
	Vector3 laPos = position;
	laPos += Vector3(width * -0.65f, torsoSize.y * 0.8f, 0);
	b->init(device, width * 0.2f, height * 0.37f, depth * 0.5f, LightBlue, LightBlue);
	b->setDiffuseMap(diffuseMapVar);
	leftArm->init("leftArm", b, laPos, direction, Vector3(width * 0.2f, height * 0.37f, depth * 0.5f), speed);
	leftArm->setBody(this);
	leftArm->setRoot(torso);

	// right leg
	b = new Box();
	Vector3 rlPos = position;
	rlPos += Vector3(width * 0.35f, height * -0.02f, 0);
	b->init(device, width * 0.25f, height * 0.21f, depth * 0.5f, LightBlue, LightBlue);
	b->setDiffuseMap(diffuseMapVar);
	rightLeg->init("rightLeg", b, rlPos, direction, Vector3(width * 0.25f, height * 0.21f, depth * 0.5f), speed);
	rightLeg->setBody(this);
	rightLeg->setRoot(torso);

	// left leg
	b = new Box();
	Vector3 llPos = position;
	llPos += Vector3(width * -0.35f, height * -0.02f, 0);
	b->init(device, width * 0.25f, height * 0.21f, depth * 0.5f, LightBlue, LightBlue);
	b->setDiffuseMap(diffuseMapVar);
	leftLeg->init("leftLeg", b, llPos, direction, Vector3(width * 0.25f, height * 0.21f, depth * 0.5f), speed);
	leftLeg->setBody(this);
	leftLeg->setRoot(torso);

	//right shin
	b = new Box();
	Vector3 rsPos = position;
	rsPos += Vector3(0, height * .22f, 0);
	b->init(device, width * 0.24f, height * 0.20f, depth * 0.5f, LightBlue, LightBlue);
	b->setDiffuseMap(diffuseMapVar);
	rightShin->init("rightShin", b, rsPos, direction, Vector3(width * 0.24f, height * 0.20f, depth * 0.5f), speed);
	rightShin->setBody(this);
	rightShin->setRoot(rightLeg);

	//left shin
	b = new Box();
	Vector3 lsPos = position;
	lsPos += Vector3(0, height * .22f, 0);
	b->init(device, width * 0.24f, height * 0.20f, depth * 0.5f, LightBlue, LightBlue);
	b->setDiffuseMap(diffuseMapVar);
	leftShin->init("leftShin", b, lsPos, direction, Vector3(width * 0.24f, height * 0.20f, depth * 0.5f), speed);
	leftShin->setBody(this);
	leftShin->setRoot(leftLeg);

	// connect body parts
	torso->addChild(head);
	torso->addChild(rightArm);
	torso->addChild(leftArm);
	torso->addChild(rightLeg);
	torso->addChild(leftLeg);
	rightLeg->addChild(rightShin);
	leftLeg->addChild(leftShin);
	b = 0;
}

void Player::update(float dt)
{
	gameTime += dt;
	elapsed += dt;
	lastMousePos = mousePos;
	mousePos = Vector2(input->getMouseRawX(), input->getMouseRawY());
	//Take care of input
	//torso->setSpeed(0.0f);
	bool moving = false;
	bool sprinting = false;
	if (keyPressed(PlayerSprintKey))
	{
		speed = sprintBoost;
		sprinting = true;
	}
	else
	{
		speed = normalSpeed;
	}
	if (keyPressed(PlayerForwardKey))
	{
		//torso->setSpeed(speed);
		position += direction * speed * dt;
		if (position.y != 0)
			position.y = 0;
		for (int i = 0; i < perimeter.size(); i++) {
			perimeter[i] += direction * speed * dt;
			if (perimeter[i].y != 0)
				perimeter[i].y = 0;
		}
		moving = true;
	}
	if (keyPressed(PlayerBackKey))
	{
		position -= direction * speed * dt;
		for (int i = 0; i < perimeter.size(); i++) {
			perimeter[i] -= direction * speed * dt;
		}
		moving = true;
	}
	if (!moving)
		elapsed = 0;
	if (canStrafe)
	{
		if (keyPressed(PlayerRightKey))
		{
			Vector3 rightStrafe;
			Cross(&rightStrafe, &up, &direction);
			position += rightStrafe * normalSpeed * dt;
			for (int i = 0; i < perimeter.size(); i++) {
				perimeter[i] += rightStrafe * normalSpeed * dt;
			}
		}
		if (keyPressed(PlayerLeftKey))
		{
			Vector3 leftStrafe;
			Cross(&leftStrafe, &direction, &up);
			position += leftStrafe * normalSpeed * dt;
			for (int i = 0; i < perimeter.size(); i++) {
				perimeter[i] += leftStrafe * normalSpeed * dt;
			}
		}
	}
	//dirTheta += float(mousePos.x - lastMousePos.x)* 100.0f * dt;
	dirTheta += float(mousePos.x) * dt;

	if (sprinting && moving && !colliding)
		torso->setRotX(ToRadian(15));
	else
		torso->setRotX(ToRadian(0));

	direction.x = sinf(dirTheta);
	direction.z = cosf(dirTheta);
	torso->setDirection(direction);
	torso->setPosition(Vector3(position.x, position.y + height * 0.5f, position.z));


	spotLight->pos = torso->getPosition() + direction * depth + Vector3(0, head->getPosition().y, 0);
	Vector3 lightTarget = position + direction * 70;
	Vector3 lightLookAt = lightTarget - spotLight->pos;
	//spotLight->pos.y += 10.0f;
	//Vector3 normalizedDir = (torso->getDirection()*12)-torso->getPosition();
	D3DXVec3Normalize(&spotLight->dir, &(lightLookAt));
	

	//	leg movement
	float normPos = 175;
	float legRot = sin(elapsed * limbSpeed);
	float phase = 0;
	float shinRot = sin(elapsed * limbSpeed + phase);
	float legRange = 0;
	float legOffset = 0;
	float shinRange = 0;
	float shinOffset = 10;
	if (!colliding)
	{
		if (moving && !sprinting) // set the leg position variables
		{	//swing legs back and forth if moving
			normPos = 175;
			legRot = sin(elapsed * limbSpeed);
			phase = 1.414f;
			shinRot = sin(elapsed * limbSpeed + phase);
			legRange = 20;
			legOffset = 0;
			shinRange = 25;
			shinOffset = 15;
		}
		else if (moving && sprinting)
		{
			normPos = 175;
			legRot = sin(elapsed * limbSpeed);
			phase = 1.414f;
			shinRot = sin(elapsed * limbSpeed + phase);
			legRange = 90;
			legOffset = -45;
			shinRange = 80;
			shinOffset = 55;
		}
	}
	// set the leg positions
	rightLeg->setRotX(ToRadian(normPos + (-legRot * legRange + legOffset)));
	leftLeg->setRotX(ToRadian(normPos + (legRot * legRange + legOffset)));

	rightShin->setRotX(ToRadian(shinRot * shinRange + shinOffset));
	leftShin->setRotX(ToRadian(-shinRot * shinRange + shinOffset));


	if (keyPressed(PlayerJumpKey))
	{
		torso->reduceScaleByFactor(1.01f);
		height /= 1.01f;
	}
	
	//	arm movement
	// rotate arms down
	rightArm->setRotX(ToRadian(180));
	leftArm->setRotX(ToRadian(180));
	if (!colliding)
	{
		if (moving && !sprinting)
		{	//swing arms back and forth if moving
			float normPos = 180;
			float armRot = sin(elapsed * limbSpeed);
			float armRange = 10;
			rightArm->setRotX(ToRadian(normPos + (armRot * armRange)));
			leftArm->setRotX(ToRadian(normPos + (-armRot * armRange)));
		}
		else if (moving && sprinting)
		{
			float normPos = 180;
			float armRot = sin(elapsed * limbSpeed);
			float armRange = 30;
			float armOffset = 10;
			rightArm->setRotX(ToRadian(normPos + (armRot * armRange) + armOffset));
			leftArm->setRotX(ToRadian(normPos + (-armRot * armRange) + armOffset));
		}
		else
		{	//move arms by side if not moving
			rightArm->setRotX(ToRadian(180));
			leftArm->setRotX(ToRadian(180));
		}
	}

	//Update the bodyparts
	head->update(dt);
	rightArm->update(dt);
	leftArm->update(dt);
	rightLeg->update(dt);
	leftLeg->update(dt);
	rightShin->update(dt);
	leftShin->update(dt);
	torso->update(dt);
	
	colliding = false;

}

void Player::draw(Matrix mVP)
{
	D3D10_TECHNIQUE_DESC techDesc;
    mTech->GetDesc( &techDesc );
    for(UINT p = 0; p < techDesc.Passes; ++p)
    {
		torso->draw(mVP);
    }


}

void Player::setPosition(Vector3 pos) {

	position = pos;
	perimeter[0] = Vector3(pos.x - width/2 - 1.4f, 0.1f, pos.z - depth/2 - 1.4f);
	perimeter[1] = Vector3(pos.x, 0.1f, pos.z - depth/2 - 1.4f);
	perimeter[2] = Vector3(pos.x + width/2 + 1.4f, 0.1f, pos.z - depth/2 - 1.4f);
	perimeter[3] = Vector3(pos.x + width/2 + 1.4f, 0.1f, pos.z);
	perimeter[4] = Vector3(pos.x + width/2 + 1.4f, 0.1f, pos.z + depth/2 + 1.4f);
	perimeter[5] = Vector3(pos.x, 0.1f, pos.z + depth/2 + 1.4f);
	perimeter[6] = Vector3(pos.x - width/2 - 1.4f, 0.1f, pos.z + depth/2 + 1.4f);
	perimeter[7] = Vector3(pos.x - width/2 - 1.4f, 0.1f, pos.z);
}

void Player::setDiffuseMap(ID3D10EffectShaderResourceVariable* var)
{
	diffuseMapVar = var;
}

void Player::setLightingVar(Light* light) {
	spotLight = light;
}