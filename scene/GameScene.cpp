#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include<time.h>
using namespace DirectX;

GameScene::GameScene() {}

GameScene::~GameScene() {
	delete spriteBG_;
	delete modelStage_;
	delete modelPlayer_;
	delete modelBeam_;
	delete modelEnemy_;
}

void GameScene::Initialize() {

	srand((unsigned int)time(NULL));
	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	debugText_ = DebugText::GetInstance();

	// BG(2D)
	textureHandleBG_ = TextureManager::Load("bg.jpg");
	spriteBG_ = Sprite::Create(textureHandleBG_, {0, 0});

	//ビュープロジェクション初期化
	viewProjection_.eye = {0, 1, -6};
	viewProjection_.target = {0, 1, 0};
	viewProjection_.Initialize();

	//ステージ
	textureHandleStage_ = TextureManager::Load("stage.jpg");
	modelStage_ = Model::Create();
	worldTransformStage_.translation_ = {0, -1.5f, 0};
	worldTransformStage_.scale_ = {4.5f, 1, 40};
	worldTransformStage_.Initialize();

	//プレイヤー
	textureHandlePlayer_ = TextureManager::Load("player.png");
	modelPlayer_ = Model::Create();
	worldTransformPlayer_.scale_ = {0.5f, 0.5f, 0.5f};
	worldTransformPlayer_.Initialize();

	//ビーム
	textureHandleBeam_ = TextureManager::Load("beam.png");
	worldTransformBeam_.scale_ = {0.3f, 0.3f, 0.3f};
	worldTransformBeam_.Initialize();
	modelBeam_ = Model::Create();

	//敵
	textureHandleEnemy_ = TextureManager::Load("enemy.png");
	modelEnemy_ = Model::Create();
	worldTransformEnemy_.scale_ = {0.5f, 0.5f, 0.5f};
	worldTransformEnemy_.Initialize();
}

void GameScene::Update() {
	PlayerUpdate();
	BeamUpdate();
	EnemyUpdate();
	CollisionUpdate();
}
//プレイヤー
void GameScene::PlayerUpdate() {
	//移動
	//右
	if (input_->PushKey(DIK_RIGHT)) {
		worldTransformPlayer_.translation_.x += 0.1f;
		if (worldTransformPlayer_.translation_.x > 4) {
			worldTransformPlayer_.translation_.x = 4;
		}
	}
	//左
	if (input_->PushKey(DIK_LEFT)) {
		worldTransformPlayer_.translation_.x -= 0.1f;
		if (worldTransformPlayer_.translation_.x < -4) {
			worldTransformPlayer_.translation_.x = -4;
		}
	}
	//行列更新
	worldTransformPlayer_.UpdateMatrix();
}

//ビーム更新
void GameScene::BeamUpdate() {
	//移動
	BeamMove();
	//
	BeamBorn();
	//行列更新
	worldTransformBeam_.UpdateMatrix();
}

//ビーム移動
void GameScene::BeamMove() {
	if (beamflag_ == 1) {
		worldTransformBeam_.translation_.z += 0.3f;
		worldTransformBeam_.rotation_.x += 0.3f;
		if (worldTransformBeam_.translation_.z > 40) {
			beamflag_ = 0;
		}
	}
}
void GameScene::BeamBorn() {
	if (beamflag_ == 0) {
		if (input_->PushKey(DIK_SPACE)) {
			beamflag_ = 1;
			worldTransformBeam_.translation_.x = worldTransformPlayer_.translation_.x;
			worldTransformBeam_.translation_.z = worldTransformPlayer_.translation_.z;
		}
	}
}

//敵更新
void GameScene::EnemyUpdate() {
	//移動
	EnemyMove();

	enemyBorn();
	//行列更新
	worldTransformEnemy_.UpdateMatrix();
}

//敵移動
void GameScene::EnemyMove() {
	if (enemyFlag_ == 1) {
		worldTransformEnemy_.translation_.z -= 0.7f;
		worldTransformEnemy_.rotation_.x -= 0.1f;
	}
	if (worldTransformEnemy_.translation_.z < -5) {
		enemyFlag_ = 0;
	}
	if (enemyFlag_ == 0) {
		worldTransformEnemy_.translation_.z = 40.0f;
		enemyFlag_ = 1;
	}
}
void GameScene::enemyBorn() {
	if (enemyFlag_ == 0) {
		enemyFlag_ = 1;
		worldTransformEnemy_.translation_.z =40.0f;
		//	if (worldTransformEnemy_.translation_.z < -5) {
		int x = rand() % 80;
		float x2 = (float)x / 10 - 4;
		worldTransformEnemy_.translation_.x = x2;
//		worldTransformEnemy_.translation_.z = 40.0f;
	}
}
void GameScene::CollisionUpdate() {
	Collision();
	CollisionPlayerEnemy();
}

//衝突判定
void GameScene::Collision() {
	CollisionPlayerEnemy();
//	CollisionBeamEnemy();
}
void GameScene::CollisionPlayerEnemy() {
	if (enemyFlag_ == 1) {
		float dx = abs(worldTransformPlayer_.translation_.x - worldTransformEnemy_.translation_.x);
		float dz = abs(worldTransformPlayer_.translation_.z - worldTransformEnemy_.translation_.z);
		//衝突したら
		if (dx < 1 && dz < 1) {

			enemyFlag_ = 0;
		}
	}
}
//void GameScene::CollisionBeamEnemy() {
//	if (enemyFlag_ == 1) {
//		float dx = abs(worldTransformBeam_.translation_.x - worldTransformEnemy_.translation_.x);
//		float dz = abs(worldTransformBeam_.translation_.z - worldTransformEnemy_.translation_.z);
		//衝突したら
//		if (dx < 1 && dz < 1) {

//			enemyFlag_ = 0;
//		}
//	}
//}

void GameScene::Draw() {
	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>
	spriteBG_->Draw();
	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>
	modelStage_->Draw(worldTransformStage_, viewProjection_, textureHandleStage_);

	modelPlayer_->Draw(worldTransformPlayer_, viewProjection_, textureHandlePlayer_);

	if (beamflag_ == 1) {
		modelBeam_->Draw(worldTransformBeam_, viewProjection_, textureHandleBeam_);
	}

	modelEnemy_->Draw(worldTransformEnemy_, viewProjection_, textureHandleEnemy_);
	// 3Dオブジェクト描画後処理
	Model::PostDraw();
	//スコア
	char str[100];
	sprintf_s(str, "SCORE %d", gameScore_);
	debugText_->Print(str, 200, 10, 2);
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// デバッグテキストの描画
	debugText_->DrawAll(commandList);
	//
	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}
