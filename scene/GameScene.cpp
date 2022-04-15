#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>

using namespace DirectX;

//コンストラクタ
GameScene::GameScene() {}

//デストラクタ
GameScene::~GameScene() {
	delete sprite_;
	delete model_;
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	debugText_ = DebugText::GetInstance();

	textureHandle_ = TextureManager::Load("mario.jpg");
	soundDataHandle_ = audio_->LoadWave("se_sad03.wav");
	voiceHandle_ = audio_->PlayWave(soundDataHandle_, true);

	sprite_ = Sprite::Create(textureHandle_, {100, 50});
	audio_->PlayWave(soundDataHandle_);

	model_ = Model::Create();

	worldTransform_.Initialize();
	viewProjection_.Initialize();
}
//更新
void GameScene::Update() {
	if (input_->TriggerKey(DIK_SPACE))
		//音声停止
		audio_->StopWave(voiceHandle_);

	//	debugText_->Print("ikisugii 19 19", 50, 50, 1.0f);
	//	debugText_->SetPos(50, 70);
	//	debugText_->Printf("year:%d", 2001);

	value_++;

	std::string strDebug = std::string("value") + std::to_string(value_);

	debugText_->Print(strDebug, 50, 50, 1.0f);
}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

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

	model_->Draw(worldTransform_, viewProjection_, textureHandle_);

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>
	sprite_->Draw();

	// デバッグテキストの描画
	debugText_->DrawAll(commandList);
	//
	// スプライト描画後処理
	Sprite::PostDraw();
	//スプライトの今の座標を取得
	XMFLOAT2 position = sprite_->GetPosition();
	//座標を｛2,0｝移動
	position.x += 2.0f;
	position.y += 1.0f;
	//移動した座標をスプライトに反映
	sprite_->SetPosition(position);

#pragma endregion
}