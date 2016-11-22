#ifndef _RESOURCE_H_
#define _RESOURCE_H_

//Font
static const char s_font[] = "fonts/panpizza.ttf";
const float s_font_ratio= 1;

//Splash Scene elements
static const char s_logo[] = "splashlogo.png";

//Home Scene elements
static const char s_homescene_background[] = "homescene/bg_home.png";
static const char s_homescene_menu_bar[] = "homescene/menu_bar.png";
static const char s_homescene_progress[] = "homescene/progress.png";
static const char s_homescene_bg_progress[] = "homescene/bg_progress.png";
static const char s_homescene_sprite_time_to_get_iap_sticker[] = "homescene/sprite_time_to_get_iap_sticker.png";
static const char s_homescene_sprite_time_to_get_free_sticker[] = "homescene/sprite_time_to_get_free_sticker.png";
static const char s_homescene_btn_sticker[] = "homescene/btn_sticker.png";
static const char s_homescene_btn_home[] = "homescene/btn_home.png";
static const char s_homescene_btn_setting[] = "homescene/btn_setting.png";
static const char s_homescene_btn_rating[] = "homescene/btn_rating.png";
static const char s_homescene_btn_more_game[] = "homescene/btn_more_game.png";
static const char s_homescene_btn_facebook_page[] = "homescene/btn_facebook_page.png";
static const char s_homescene_btn_album[] = "homescene/btn_album.png";
static const char s_homescene_btn_friend[] = "homescene/btn_friend.png";
static const char s_homescene_btn_trade[] = "homescene/btn_trade.png";
static const char s_homescene_btn_rewarded_ads[] = "homescene/btn_rewarded_ads.png";
static const char s_homescene_btn_iap[] = "homescene/btn_iap.png";
static const char s_homescene_btn_continue[] = "homescene/btn_continue.png";

static char* s_homescene_cut_sheet[] = {"homescene/cut_1.png","homescene/cut_2.png"};
static const char s_homescene_btn_free_packet_top[] = "homescene/free_packet_top.png";
static const char s_homescene_btn_free_packet_bottom[] = "homescene/free_packet_bottom.png";
static const char s_homescene_btn_common_packet_top[] = "homescene/common_packet_top.png";
static const char s_homescene_btn_common_packet_bottom[] = "homescene/common_packet_bottom.png";
static const char s_homescene_btn_uncommon_packet_top[] = "homescene/uncommon_packet_top.png";
static const char s_homescene_btn_uncommon_packet_bottom[] = "homescene/uncommon_packet_bottom.png";
static const char s_homescene_btn_rare_packet_top[] = "homescene/rare_packet_top.png";
static const char s_homescene_btn_rare_packet_bottom[] = "homescene/rare_packet_bottom.png";
static const char s_homescene_btn_bundle_packet_top[] = "homescene/bundle_packet_top.png";
static const char s_homescene_btn_bundle_packet_bottom[] = "homescene/bundle_packet_bottom.png";
static const char s_homescene_new[] = "homescene/sprite_new.png";
static const char s_homescene_uncommon[] = "homescene/sprite_uncommon.png";


//Setting Scene elements
static const char s_settingscene_background[] = "settingscene/bg_setting.png";
static const char s_settingscene_btn_back[] = "settingscene/btn_back.png";
static const char s_settingscene_btn_loggout[] = "settingscene/btn_loggout.png";
static const char s_settingscene_rdb_selected[] = "settingscene/rdb_selected.png";
static const char s_settingscene_rdb_unselected[] = "settingscene/rdb_unselected.png";

//Sticker Scene elements
static const char s_stickerscene_background[] = "stickerscene/bg_sticker.png";
static const char s_stickerscene_btn_home[] = "stickerscene/btn_home.png";
static const char s_stickerscene_btn_album[] = "stickerscene/btn_album.png";
static const char s_stickerscene_btn_sticker[] = "stickerscene/btn_sticker.png";
static const char s_stickerscene_btn_stick[] = "stickerscene/btn_stick.png";
static const char s_stickerscene_btn_all[] = "stickerscene/btn_all.png";
static const char s_stickerscene_btn_sell[] = "stickerscene/btn_sell.png";
static const char s_stickerscene_menu_bottom[] = "stickerscene/bottom_bar.png";
static const char s_stickerscene_sprite_exclamation[] = "stickerscene/sprite_exclamation.png";
static const char s_stickerscene_sprite_question[] = "stickerscene/sprite_question.png";
static const char s_stickerscene_sprite_search[] = "stickerscene/sprite_search.png";
static const char s_stickerscene_sprite_sticker_detail[] = "stickerscene/sticker_detail.png";
static const char s_stickerscene_sprite_stick[] = "stickerscene/stick.png";


//Album Scene elements
static const char s_albumscene_background[] = "albumscene/bg_album.png";
static const char s_albumscene_btn_sticker[] = "albumscene/btn_sticker.png";
static const char s_albumscene_btn_home[] = "albumscene/btn_home.png";
static const char s_albumscene_btn_album[] = "albumscene/btn_album.png";


//Sound
static const char s_gameon[] = "sound/game_on.mp3";
static const char s_click[] = "sound/click.mp3";

//Preload all resouces
static char* s_imgloading[] ={"rankingscene/loading_apple.png","rankingscene/number.png","rankingscene/bg_ranking.png","rankingscene/btn_home.png","rankingscene/btn_play.png","rankingscene/board.png","rankingscene/my_number.png","rankingscene/txt_learderboard.png","rankingscene/btn_connectFB_LogIn.png","rankingscene/btn_connectFB_LogOut.png","rankingscene/btn_friendMode.png","rankingscene/btn_friendMode_CLicked.png","rankingscene/btn_worldMode.png","rankingscene/btn_worldMode_CLicked.png","gamescene/tutorial.png","gamescene/animation/player_fall/player_fall1.png","gamescene/animation/player_fall/player_fall2.png","gamescene/animation/player_fall/player_fall3.png","gamescene/animation/player_fall/player_fall4.png","gamescene/animation/player_fall/player_fall5.png","gamescene/animation/player_fall/player_fall6.png","gamescene/animation/water/water/water1.png","gamescene/animation/water/water/water2.png","gamescene/animation/water/water/water3.png","gamescene/animation/water/water/water4.png","gamescene/animation/water/water/water5.png","gamescene/animation/water/water/water6.png","gamescene/animation/water/water_land/water_land1.png","gamescene/animation/water/water_land/water_land2.png","gamescene/animation/water/water_land/water_land3.png","gamescene/animation/water/water_land/water_land4.png","gamescene/steps_1.png","gamescene/steps_2.png","storyline/bg_story.png","storyline/str_1.png","storyline/str_2.png","storyline/str_3.png","storyline/quote.png","gamescene/animation/enemy/enemy1.png","gamescene/animation/enemy/enemy2.png","gamescene/animation/enemy/enemy3.png","gamescene/animation/enemy/enemy4.png","gamescene/animation/enemy/enemy5.png","gamescene/animation/enemy/enemy6.png","gamescene/animation/enemy/enemy7.png","gamescene/animation/player/player1.png","gamescene/animation/player/player2.png","gamescene/animation/player/player3.png","gamescene/animation/player/player4.png","gamescene/animation/player/player5.png","gamescene/animation/player/player6.png","dialogs/btn_123.png","dialogs/btn_face.png","dialogs/btn_home.png","dialogs/btn_play.png","dialogs/btn_replay.png","dialogs/paper.png","dialogs/txt_GameOver.png","dialogs/txt_Pause.png","dialogs/sketch_gameover.png","dialogs/sketch_pause.png","gamescene/bonus.png","gamescene/player.png","gamescene/enemy.png","gamescene/paper_score.png","gamescene/btn_pause.png","gamescene/eyes_1.png","gamescene/eyes_2.png","gamescene/land_real_dangerous.png","gamescene/land_real_none.png","gamescene/bg_playing.png","mainmenu/txt_name.png","mainmenu/btn_sound_off.png","mainmenu/btn_sound_on.png","mainmenu/btn_play.png","mainmenu/btn_more.png","mainmenu/bg_menu.png","gamescene/land_3.png","gamescene/land_2.png","gamescene/land_1.png","gamescene/land_real.png"};
static char* s_soundloading[] = {"sound/game_on.mp3","sound/click.mp3"};
#endif
