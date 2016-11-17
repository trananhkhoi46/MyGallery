#ifndef _RESOURCE_H_
#define _RESOURCE_H_


//Global KEY
static const char SOUND[] = "isSound";
static const char SCORE[] = "score";
static const char HIGHSCORE[] = "highscore";
static const char RANKINGWORLDMODE[] = "rankingmode";

//Resources key to use in game
static const char s_logo[] = "splashlogo.png";

//Font
static const char s_font_1[] = "fonts/panpizza.ttf";
static const char s_font_2[] = "fonts/Raanana.ttf";
const float s_font_ratio= 1;

//Ads layer
static const char s_adslayer_background[] = "ads/ads_background.png";
static const char s_adslayer_closebtn[] = "ads/close.png";

//Main menu elements
static const char s_bgmenu[] = "mainmenu/bg_menu.png";
static const char s_like_btn[] = "mainmenu/btn_like.png";
static const char s_more_btn[] = "mainmenu/btn_more.png";
static const char s_mmplay_btn[] = "mainmenu/btn_play.png";
static const char s_sound_on_btn[] = "mainmenu/btn_sound_on.png";
static const char s_sound_off_btn[] = "mainmenu/btn_sound_off.png";
static const char s_title[] = "mainmenu/txt_name.png";

//Story Scene elements
static const char s_bgstory[] = "storyline/bg_story.png";
static const char s_story1[] = "storyline/str_1.png";
static const char s_story2[] = "storyline/str_2.png";
static const char s_story3[] = "storyline/str_3.png";
static const char s_story_quote[] = "storyline/quote.png";
static const char s_story_playbtn[] = "storyline/btn_play.png";

//Ranking Scene elements
static const char s_ranking_title[] = "rankingscene/txt_learderboard.png";
static const char s_ranking_myNumber[] = "rankingscene/my_number.png";
static const char s_ranking_number[] = "rankingscene/number.png";
static const char s_ranking_board[] = "rankingscene/board.png";
static const char s_ranking_btnHome[] = "rankingscene/btn_home.png";
static const char s_ranking_btnPlay[] = "rankingscene/btn_play.png";
static const char s_ranking_background[] = "rankingscene/bg_ranking.png";
static const char s_ranking_loadingSprite[] = "rankingscene/loading.png";
static const char s_ranking_loadingSprite_child[] = "rankingscene/loading_apple.png";
static const char s_ranking_btnWorldMode[] = "rankingscene/btn_worldMode.png";
static const char s_ranking_btnWorldMode_Clicked[] = "rankingscene/btn_worldMode_CLicked.png";
static const char s_ranking_btnFriendMode[] = "rankingscene/btn_friendMode.png";
static const char s_ranking_btnFriendMode_CLicked[] = "rankingscene/btn_friendMode_Clicked.png";
static const char s_ranking_btnConnectFacebook_LogOut[] = "rankingscene/btn_connectFB_LogOut.png";
static const char s_ranking_btnConnectFacebook_LogIn[] = "rankingscene/btn_connectFB_LogIn.png";

//Game Scene elements
static const char s_tutorial[] = "gamescene/tutorial.png";
static const char s_bonus[] = "gamescene/bonus.png";
static const char s_player[] = "gamescene/player.png";
static char* s_player_sheet[] = {"gamescene/animation/player/player1.png","gamescene/animation/player/player2.png",
"gamescene/animation/player/player3.png","gamescene/animation/player/player4.png","gamescene/animation/player/player5.png","gamescene/animation/player/player6.png"};
static char* s_player_fall_sheet[] = {"gamescene/animation/player_fall/player_fall1.png","gamescene/animation/player_fall/player_fall2.png",
"gamescene/animation/player_fall/player_fall3.png","gamescene/animation/player_fall/player_fall4.png","gamescene/animation/player_fall/player_fall5.png","gamescene/animation/player_fall/player_fall6.png"};
static const char s_enemy[] = "gamescene/enemy.png";
static char* s_enemy_sheet[] = {"gamescene/animation/enemy/enemy1.png","gamescene/animation/enemy/enemy2.png",
"gamescene/animation/enemy/enemy3.png","gamescene/animation/enemy/enemy4.png","gamescene/animation/enemy/enemy5.png",
"gamescene/animation/enemy/enemy6.png","gamescene/animation/enemy/enemy7.png"};
static char* s_wave_sheet[] = {"gamescene/animation/water/water/water1.png","gamescene/animation/water/water/water2.png",
"gamescene/animation/water/water/water3.png","gamescene/animation/water/water/water4.png","gamescene/animation/water/water/water5.png",
"gamescene/animation/water/water/water6.png"};
static char* s_wave_land_sheet[] = {"gamescene/animation/water/water_land/water_land1.png","gamescene/animation/water/water_land/water_land2.png","gamescene/animation/water/water_land/water_land3.png","gamescene/animation/water/water_land/water_land4.png"};
static const char s_paper_score[] = "gamescene/paper_score.png";
static const char s_btn_pause[] = "gamescene/btn_pause.png";
static const char s_bgGameScene[] = "gamescene/bg_playing.png";
static const char s_eyes1[] = "gamescene/eyes_1.png";
static const char s_eyes2[] = "gamescene/eyes_2.png";
static const char s_realLand[] = "gamescene/land_real.png";
static const char s_realLand_none[] = "gamescene/land_real_none.png";
static const char s_realLand_dangerous[] = "gamescene/land_real_dangerous.png";
static const char s_decoratingLand1[] = "gamescene/land_1.png";
static const char s_decoratingLand2[] = "gamescene/land_2.png";
static const char s_decoratingLand3[] = "gamescene/land_3.png";
static const char s_step1[] = "gamescene/steps_1.png";
static const char s_step2[] = "gamescene/steps_2.png";

//Pause and G.O dialogs elements
static const char s_ranking_btn[] = "dialogs/btn_123.png";
static const char s_fb_btn[] = "dialogs/btn_face.png";
static const char s_home_btn[] = "dialogs/btn_home.png";
static const char s_play_btn[] = "dialogs/btn_play.png";
static const char s_replay_btn[] = "dialogs/btn_replay.png";
static const char s_dialog[] = "dialogs/paper.png";
static const char s_gameovertext[] = "dialogs/txt_GameOver.png";
static const char s_pausetext[] = "dialogs/txt_Pause.png";
static const char s_gameoverimage[] = "dialogs/sketch_gameover.png";
static const char s_pauseimage[] = "dialogs/sketch_pause.png";
const int kTagLayerHavingPauseDialog = 1996;

//Sound
static const char s_gameon[] = "sound/game_on.mp3";
static const char s_click[] = "sound/click.mp3";

//Preload all resouces
static char* s_imgloading[] ={"rankingscene/loading_apple.png","rankingscene/number.png","rankingscene/bg_ranking.png","rankingscene/btn_home.png","rankingscene/btn_play.png","rankingscene/board.png","rankingscene/my_number.png","rankingscene/txt_learderboard.png","rankingscene/btn_connectFB_LogIn.png","rankingscene/btn_connectFB_LogOut.png","rankingscene/btn_friendMode.png","rankingscene/btn_friendMode_CLicked.png","rankingscene/btn_worldMode.png","rankingscene/btn_worldMode_CLicked.png","gamescene/tutorial.png","gamescene/animation/player_fall/player_fall1.png","gamescene/animation/player_fall/player_fall2.png","gamescene/animation/player_fall/player_fall3.png","gamescene/animation/player_fall/player_fall4.png","gamescene/animation/player_fall/player_fall5.png","gamescene/animation/player_fall/player_fall6.png","gamescene/animation/water/water/water1.png","gamescene/animation/water/water/water2.png","gamescene/animation/water/water/water3.png","gamescene/animation/water/water/water4.png","gamescene/animation/water/water/water5.png","gamescene/animation/water/water/water6.png","gamescene/animation/water/water_land/water_land1.png","gamescene/animation/water/water_land/water_land2.png","gamescene/animation/water/water_land/water_land3.png","gamescene/animation/water/water_land/water_land4.png","gamescene/steps_1.png","gamescene/steps_2.png","storyline/bg_story.png","storyline/str_1.png","storyline/str_2.png","storyline/str_3.png","storyline/quote.png","gamescene/animation/enemy/enemy1.png","gamescene/animation/enemy/enemy2.png","gamescene/animation/enemy/enemy3.png","gamescene/animation/enemy/enemy4.png","gamescene/animation/enemy/enemy5.png","gamescene/animation/enemy/enemy6.png","gamescene/animation/enemy/enemy7.png","gamescene/animation/player/player1.png","gamescene/animation/player/player2.png","gamescene/animation/player/player3.png","gamescene/animation/player/player4.png","gamescene/animation/player/player5.png","gamescene/animation/player/player6.png","dialogs/btn_123.png","dialogs/btn_face.png","dialogs/btn_home.png","dialogs/btn_play.png","dialogs/btn_replay.png","dialogs/paper.png","dialogs/txt_GameOver.png","dialogs/txt_Pause.png","dialogs/sketch_gameover.png","dialogs/sketch_pause.png","gamescene/bonus.png","gamescene/player.png","gamescene/enemy.png","gamescene/paper_score.png","gamescene/btn_pause.png","gamescene/eyes_1.png","gamescene/eyes_2.png","gamescene/land_real_dangerous.png","gamescene/land_real_none.png","gamescene/bg_playing.png","mainmenu/txt_name.png","mainmenu/btn_sound_off.png","mainmenu/btn_sound_on.png","mainmenu/btn_play.png","mainmenu/btn_more.png","mainmenu/bg_menu.png","gamescene/land_3.png","gamescene/land_2.png","gamescene/land_1.png","gamescene/land_real.png"};
static char* s_soundloading[] = {"sound/game_on.mp3","sound/click.mp3"};



#endif
