#pragma once

#include "Audio.h"
#include "DebugText.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include <DirectXMath.h>

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {

  public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	//プレイヤー更新
	void PlayerUpdate();

	//ビーム更新
	void BeamUpdate();

	//ビーム移動
	void BeamMove();

	//ビーム発生
	void BeamBorn();

	//ビーム存在フラグ
	int beamflag_ = 0;

	//敵生存フラグ
	int enemyFlag_ = 0;
	void enemyBorn();

	//衝突判定
	void Collision();
	void CollisionPlayerEnemy();
	//衝突　ビームと敵
//	void CollisionBeamEnemy();

	//スコア
	int gameScore_ = 0;

  private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;
	DebugText* debugText_ = nullptr;

	/// <summary>
	/// ゲームシーン用
	/// </summary>

	// BG
	uint32_t textureHandleBG_ = 0;
	Sprite* spriteBG_ = nullptr;
	//ビュープロジェクション
	ViewProjection viewProjection_;
	//ステージ
	uint32_t textureHandleStage_ = 0;
	Model* modelStage_ = nullptr;
	WorldTransform worldTransformStage_;

	//プレイヤー
	uint32_t textureHandlePlayer_ = 0;
	Model* modelPlayer_ = nullptr;
	WorldTransform worldTransformPlayer_;

	//ビーム
	uint32_t textureHandleBeam_ = 0;
	Model* modelBeam_ = nullptr;
	WorldTransform worldTransformBeam_;

	//敵
	uint32_t textureHandleEnemy_ = 0;
	Model* modelEnemy_ = nullptr;
	WorldTransform worldTransformEnemy_;
	//
	void EnemyUpdate();
	void EnemyMove();
	void CollisionUpdate();
};
