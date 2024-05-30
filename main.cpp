   
#include "Common_Func.h"

BaseObject g_background;
TTF_Font* font_time = NULL;

bool InitData()
{
    bool success = true;
    int ret = SDL_Init(SDL_INIT_VIDEO);
    if (ret < 0) return false;
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");

    g_window = SDL_CreateWindow("Dodge Cat",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        SCREEN_WIDTH, SCREEN_HEIGHT,
        SDL_WINDOW_SHOWN);
    if (g_window == NULL)
    {
        success = false;
    }
    else
    {
        g_screen = SDL_CreateRenderer(g_window, -1, SDL_RENDERER_ACCELERATED);
        if (g_screen == NULL)
        {
            success = false;
        }
        else
        {
            SDL_SetRenderDrawColor(g_screen, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR);
            int imgFlags = IMG_INIT_PNG;
            if (!(IMG_Init(imgFlags) && imgFlags)) success = false;
        }
    }
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
    {
        printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
        success = false;
    }
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
    {
        printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
        success = false;
    }

    if (TTF_Init() == -1)
    {
        success = false;
    }

    font_time = TTF_OpenFont("resource//font//b_font.ttf", 35);
    if (font_time == NULL)
    {
        success = false;
    }
    return success;
}

bool LoadBackground()
{
    cv::Mat image = cv::imread("resource/bg01.png");
    if (image.empty()) {
        return false;
    }
    bool ret = g_background.LoadImg("resource//bg01.png", g_screen);
    if (ret == false) return false;
    return true;
}

bool LoadBackground_menu()
{
    cv::Mat image = cv::imread("resource/bg01_menu.png");
    if (image.empty()) {
        return false;
    }
    bool ret = g_background.LoadImg("resource//bg01_menu.png", g_screen);
    if (ret == false) return false;
    return true;
}

bool LoadBackground_tuto()
{
    cv::Mat image = cv::imread("resource/bg_tutorial.png");
    if (image.empty()) {
        return false;
    }
    bool ret = g_background.LoadImg("resource//bg_tutorial.png", g_screen);
    if (ret == false) return false;
    return true;
}

bool Load_Sound()
{
    bool success = true;
    meow_sound = Mix_LoadWAV("resource//G_Sound//meow1.wav");
    bonk_sound = Mix_LoadWAV("resource//G_Sound//bonk.wav");
    lose_sound = Mix_LoadWAV("resource//G_Sound//lose.wav");
    coin_sound = Mix_LoadWAV("resource//G_Sound//coin.wav");
    bg_music = Mix_LoadMUS("resource//G_Sound/yume_to_hazakura.wav");
    if (bg_music == NULL)
    {
        printf("Failed to load beat music! SDL_mixer Error: %s\n", Mix_GetError());
        success = false;
    }
    if (meow_sound == NULL || bonk_sound == NULL || lose_sound == NULL || coin_sound == NULL)
    {
        printf("Failed to load scratch sound effect! SDL_mixer Error: %s\n", Mix_GetError());
        success = false;
    }
    return success;
}
void ShowMenu()
{

}
void close()
{
    g_background.Free();
    SDL_DestroyRenderer(g_screen);
    g_screen = NULL;
    SDL_DestroyWindow(g_window);
    g_window = NULL;

    Mix_FreeChunk(meow_sound);
    Mix_FreeChunk(bonk_sound);
    Mix_FreeChunk(lose_sound);
    Mix_FreeChunk(coin_sound);

    Mix_FreeMusic(bg_music);

    meow_sound = NULL;
    bonk_sound = NULL;
    lose_sound = NULL;
    bg_music = NULL;
    coin_sound = NULL;

    IMG_Quit();
    SDL_Quit();
}

int main(int argc, char* argv[])
{
    int GAME_STATUS = STATUS_FIRST_MENU;


    ImpTimer fps_timer;
    bool focus_menu = false;

    if (InitData() == false) return -1;
    if (Load_Sound() == false) return -1;
    if (LoadBackground_menu() == false) return -1;

    //khai báo items 
    ItemsObject power_fish;
    power_fish.LoadImg("resource//Cat_PNG//fish.png", g_screen, 531, 84);
    power_fish.set_clips();
    power_fish.set_x_pos(rand() % (SCREEN_WIDTH + 500));

    //khai báo Threats
    ThreatsObject p_threat1;
    p_threat1.LoadImg("resource//Cat_PNG//ball.png", g_screen, 323, 79);
    p_threat1.set_clips();
    p_threat1.set_x_pos(rand() % (SCREEN_WIDTH + 1500));
    p_threat1.set_drop_speed(35);

    ThreatsObject p_threat2;
    p_threat2.LoadImg("resource//Cat_PNG//ball.png", g_screen, 323, 79);
    p_threat2.set_clips();
    p_threat2.set_x_pos(rand() % (SCREEN_WIDTH + 500));
    p_threat2.set_drop_speed(40);

    ThreatsObject p_threat3;
    p_threat3.LoadImg("resource//Cat_PNG//ball.png", g_screen, 323, 79);
    p_threat3.set_clips();
    p_threat3.set_x_pos(rand() % (SCREEN_WIDTH + 1000));
    p_threat3.set_drop_speed(25);

    ThreatsObject p_threat4;
    p_threat4.LoadImg("resource//Cat_PNG//ball.png", g_screen, 323, 79);
    p_threat4.set_clips();
    p_threat4.set_x_pos(rand() % (SCREEN_WIDTH + 500));
    p_threat4.set_drop_speed(50);

    ThreatsObject p_threat5;
    p_threat5.LoadImg("resource//Cat_PNG//ball.png", g_screen, 323, 79);
    p_threat5.set_clips();
    p_threat5.set_x_pos(rand() % (SCREEN_WIDTH + 500));
    p_threat5.set_drop_speed(55);

    ThreatsObject p_threat6;
    p_threat6.LoadImg("resource//Cat_PNG//ball.png", g_screen, 323, 79);
    p_threat6.set_clips();
    p_threat6.set_x_pos(rand() % (SCREEN_WIDTH + 500));
    p_threat6.set_drop_speed(60);

    //khai báo coin
    Coin_Item coin;
    coin.LoadImg("resource//Cat_PNG//Br_coin.png", g_screen, 700, 70);
    coin.set_clips();
    coin.set_x_pos(rand() % (SCREEN_WIDTH + 500));
    coin.set_drop_speed(30);

    //thi?t l?p nhân vât
    MainObject p_player;
    p_player.LoadImg("resource//Cat_PNG//walk_trc.png", g_screen);
    p_player.set_clips();

    int hp_left = 4;
    Uint32 x_time = 0;
    //Khai báo Menu Object
    MenuObject Ball_Cat;
    Ball_Cat.LoadImg("resource//Cat_PNG//menu_cat.png", g_screen);
    Ball_Cat.set_clips();
    Ball_Cat.set_x_pos(SCREEN_WIDTH * 0.5 - 70);
    Ball_Cat.set_y_pos(150);

    // khai báo TextObjects
    TextObject time_game;
    time_game.SetColor(TextObject::RED_TEXT);

    TextObject mark_game;
    mark_game.SetColor(TextObject::RED_TEXT);
    UINT mark_value = 0;

    TextObject mark_die;
    mark_die.SetColor(TextObject::WHITE_TEXT);

    TextObject hp_game;
    hp_game.SetColor(TextObject::RED_TEXT);


    TextObject continue_text;
    continue_text.SetColor(TextObject::BLACK_TEXT);

    TextObject play_text;
    play_text.SetColor(TextObject::BLACK_TEXT);

    TextObject tutorials_text;
    tutorials_text.SetColor(TextObject::BLACK_TEXT);

    TextObject exit1_text;
    exit1_text.SetColor(TextObject::BLACK_TEXT);

    TextObject home_text;
    home_text.SetColor(TextObject::WHITE_TEXT);

    TextObject exit2_text;
    exit2_text.SetColor(TextObject::WHITE_TEXT);

    //các bool ki?m tra
    bool is_quit = false;
    bool die = false;
    bool is_paused = false;
    bool Time_inc = false;
    int current_menu = 1;
    int current_end_menu = 1;
    bool restart = false;

    Uint32 Elapsed_Time = NULL;
    Uint32 StartedTime = SDL_GetTicks() / 1000;

    //Nh?c n?n
    if (Mix_PlayingMusic() == 0 && GAME_STATUS == STATUS_PLAYING)
    {
        Mix_PlayMusic(bg_music, -1);
    }
    //
    while (!is_quit)
    {
        fps_timer.start();

        while (SDL_PollEvent(&g_event) != 0)
        {
            if (g_event.type == SDL_QUIT)
            {
                is_quit = true;
            }
            else if (g_event.type == SDL_KEYDOWN && !is_paused) {
                if (g_event.key.keysym.sym == SDLK_RETURN && GAME_STATUS == STATUS_PLAYING) {
                    is_paused = true;
                    Mix_PauseMusic();

                }
                if (g_event.key.keysym.sym == SDLK_DOWN && GAME_STATUS == STATUS_FIRST_MENU)
                {
                    if (current_menu < 3) current_menu++;
                    else current_menu = 1;
                }
                if (g_event.key.keysym.sym == SDLK_UP && GAME_STATUS == STATUS_FIRST_MENU)
                {
                    if (current_menu > 1) current_menu--;
                    else current_menu = 3;
                }

                if (g_event.key.keysym.sym == SDLK_RETURN && GAME_STATUS == STATUS_FIRST_MENU)
                {
                    if (current_menu == 1) {
                        Uint32 StartPlaying = SDL_GetTicks() / 1000;
                        Elapsed_Time = Elapsed_Time + StartPlaying - StartedTime;
                        GAME_STATUS = STATUS_PLAYING;
                        if (Mix_PlayingMusic() == 0 && GAME_STATUS == STATUS_PLAYING)
                        {
                            Mix_PlayMusic(bg_music, -1);
                        }
                        if (LoadBackground() == false) return -1;
                    }

                    if (current_menu == 2) {
                        GAME_STATUS = STATUS_TUTORIAL;
                        if (LoadBackground_tuto() == false) return -1;
                    }
                    if (current_menu == 3) {
                        close();
                    }
                }
                if (g_event.key.keysym.sym == SDLK_BACKSPACE && GAME_STATUS == STATUS_TUTORIAL)
                {
                    GAME_STATUS = STATUS_FIRST_MENU;
                    if (LoadBackground_menu() == false) return -1;
                }
                if (g_event.key.keysym.sym == SDLK_RIGHT && GAME_STATUS == STATUS_PLAYING && die == true)
                {
                    if (current_end_menu == 1) current_end_menu = 2;
                    else current_end_menu = 1;
                }
                if (g_event.key.keysym.sym == SDLK_LEFT && GAME_STATUS == STATUS_PLAYING && die == true)
                {
                    if (current_end_menu == 1) current_end_menu = 2;
                    else current_end_menu = 1;
                }
                if (g_event.key.keysym.sym == SDLK_RETURN && GAME_STATUS == STATUS_PLAYING && die == true)
                {
                    if (current_end_menu == 1)
                    {

                        if (LoadBackground_menu() == false) return -1;
                        //GAME_STATUS == STATUS_FIRST_MENU;
                        die = false;
                        mark_value = 0;
                        hp_left = 4;
                        if (LoadBackground() == false) return -1;
                        if (Mix_PlayingMusic() == 0 && GAME_STATUS == STATUS_PLAYING)
                        {
                            Mix_PlayMusic(bg_music, -1);
                        }
                        restart = true;

                    }
                    else if (current_end_menu == 2) close();

                }
            }
            p_player.HandleInputAction(g_event, g_screen);

        }

        SDL_SetRenderDrawColor(g_screen, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR);

        SDL_RenderClear(g_screen);

        g_background.Render(g_screen, NULL);

        if (GAME_STATUS == STATUS_FIRST_MENU) {
            Ball_Cat.Show(g_screen);
        }
        if (die != true && GAME_STATUS == STATUS_PLAYING) {
            p_player.Show(g_screen);

            p_threat1.Show(g_screen);
            p_threat2.Show(g_screen);
            p_threat3.Show(g_screen);

            if (mark_value >= MARK1)  p_threat4.Show(g_screen);
            if (mark_value >= MARK2) p_threat5.Show(g_screen);
            if (mark_value >= MARK3) p_threat6.Show(g_screen);

            coin.Show(g_screen);
            power_fish.Show(g_screen);
        }

        if (is_paused) {
            Uint32 paused_ticks = SDL_GetTicks();
            while (is_paused) {
                while (SDL_PollEvent(&g_event) != 0) {
                    if (g_event.type == SDL_QUIT) {
                        is_quit = true;
                        is_paused = false;
                    }
                    else if (g_event.type == SDL_KEYDOWN) {
                        if (g_event.key.keysym.sym == SDLK_RETURN && GAME_STATUS == STATUS_PLAYING) {
                            is_paused = false;
                            Mix_ResumeMusic();
                            Uint32 current_ticks = SDL_GetTicks();
                            Elapsed_Time = Elapsed_Time + (current_ticks - paused_ticks) / 1000;
                        }

                    }
                }
            }
        }


        //kh?i t?o rect cho threats, nhân v?t, items, coin ?? ki?m tra va ch?m
        SDL_Rect tRect1;
        tRect1.x = p_threat1.get_x_pos();
        tRect1.y = p_threat1.get_y_pos();
        tRect1.w = p_threat1.get_width_frame();
        tRect1.h = p_threat1.get_height_frame();

        SDL_Rect tRect2;
        tRect2.x = p_threat2.get_x_pos();
        tRect2.y = p_threat2.get_y_pos();
        tRect2.w = p_threat2.get_width_frame();
        tRect2.h = p_threat2.get_height_frame();

        SDL_Rect tRect3;
        tRect3.x = p_threat3.get_x_pos();
        tRect3.y = p_threat3.get_y_pos();
        tRect3.w = p_threat3.get_width_frame();
        tRect3.h = p_threat3.get_height_frame();

        SDL_Rect tRect4;
        tRect4.x = p_threat4.get_x_pos();
        tRect4.y = p_threat4.get_y_pos();
        tRect4.w = p_threat4.get_width_frame();
        tRect4.h = p_threat4.get_height_frame();

        SDL_Rect tRect5;
        tRect5.x = p_threat5.get_x_pos();
        tRect5.y = p_threat5.get_y_pos();
        tRect5.w = p_threat5.get_width_frame();
        tRect5.h = p_threat5.get_height_frame();

        SDL_Rect tRect6;
        tRect6.x = p_threat6.get_x_pos();
        tRect6.y = p_threat6.get_y_pos();
        tRect6.w = p_threat6.get_width_frame();
        tRect6.h = p_threat6.get_height_frame();

        SDL_Rect pRect;
        pRect.x = p_player.get_x_pos();
        pRect.y = p_player.get_y_pos();
        pRect.w = p_player.get_width_frame();
        pRect.h = p_player.get_height_frame();

        SDL_Rect p_fRect;
        p_fRect.x = power_fish.get_x_pos();
        p_fRect.y = power_fish.get_y_pos();
        p_fRect.w = power_fish.get_width_frame();
        p_fRect.h = power_fish.get_height_frame();

        SDL_Rect Coin_Rect;
        Coin_Rect.x = coin.get_x_pos();
        Coin_Rect.y = coin.get_y_pos();
        Coin_Rect.w = coin.get_width_frame();
        Coin_Rect.h = coin.get_height_frame();

        bool check1 = SDLCommonFunc::CheckCollision(pRect, tRect1);
        bool check2 = SDLCommonFunc::CheckCollision(pRect, tRect2);
        bool check3 = SDLCommonFunc::CheckCollision(pRect, tRect3);
        bool check4 = SDLCommonFunc::CheckCollision(pRect, tRect4);
        bool check5 = SDLCommonFunc::CheckCollision(pRect, tRect5);
        bool check6 = SDLCommonFunc::CheckCollision(pRect, tRect6);
        bool checkp_f = SDLCommonFunc::CheckCollision(pRect, p_fRect);
        bool check_coin = SDLCommonFunc::CheckCollision(pRect, Coin_Rect);

        //ki?m tra va ch?m v?i Items
        if (checkp_f)
        {
            power_fish.set_y_pos(-10);
            power_fish.set_x_pos(rand() % SCREEN_WIDTH);
            if (hp_left < 10) hp_left++;
            mark_value += 500;
            Mix_PlayChannel(-1, meow_sound, 0);
        }
        //ki?m tra va ch?m v?i Threats
        if (check1 == true || check2 == true || check3 == true || check4 == true || check5 == true || check6 == true)
        {
            if (hp_left > 0) {
                hp_left--;
                if (check1 == true) {
                    p_threat1.set_y_pos(-10);
                    p_threat1.set_x_pos(rand() % SCREEN_WIDTH - 100);
                }
                if (check2 == true) {
                    p_threat2.set_y_pos(-10);
                    p_threat2.set_x_pos(rand() % SCREEN_WIDTH - 100);
                }
                if (check3 == true) {
                    p_threat3.set_y_pos(-10);
                    p_threat3.set_x_pos(rand() % SCREEN_WIDTH - 100);
                }
                if (check4 == true) {
                    p_threat4.set_y_pos(-10);
                    p_threat4.set_x_pos(rand() % SCREEN_WIDTH - 100);
                }
                if (check5 == true) {
                    p_threat5.set_y_pos(-10);
                    p_threat5.set_x_pos(rand() % SCREEN_WIDTH - 100);
                }
                if (check6 == true) {
                    p_threat6.set_y_pos(-10);
                    p_threat6.set_x_pos(rand() % SCREEN_WIDTH - 100);
                }
                Mix_PlayChannel(-1, bonk_sound, 0);
                continue;
            }


            SDL_Delay(100);
            if (hp_left == 0)
            {
                g_background.LoadImg("resource//Cat_PNG//youlose.png", g_screen);
                die = true;
                Mix_PlayChannel(-1, lose_sound, 0);
                lose_sound = NULL;
                Mix_HaltMusic();
                time_game.Free();
                mark_game.Free();
                hp_game.Free();
            }
        }
        //ki?m tra va ch?m v?i coin
        if (check_coin)
        {

            if (coin.get_type_of_coin() <= 2) mark_value = mark_value + 1000;
            else if (coin.get_type_of_coin() <= 15 && coin.get_type_of_coin() > 2) mark_value = mark_value + 300;
            else if (coin.get_type_of_coin() > 15 && coin.get_type_of_coin() <= 40) mark_value = mark_value + 100;
            else if (coin.get_type_of_coin() > 40) {
                mark_value = mark_value + 500;
                Time_inc = true;
            }

            Mix_PlayChannel(-1, coin_sound, 0);

            coin.set_y_pos(-10);
            coin.set_x_pos(rand() % SCREEN_WIDTH);
            coin.set_type_of_coin(rand() % 50);
            continue;
        }

        if (GAME_STATUS == STATUS_PLAYING)
        {
            //SHOW GAME TIME
//Timer text
            std::string str_time = "Time: ";
            Uint32 time_inc_val = 6;
            if (Time_inc) {
                x_time += 1;
                Time_inc = false;
            }

            if (is_paused) Uint32 pause_start_time = NULL;
            Uint32 total_time = 100;
            Uint32 time_val = SDL_GetTicks() / 1000;
            Uint32 val_time = total_time - time_val + x_time * time_inc_val + Elapsed_Time;
            if (restart == true)
            {
                if (val_time < 100) Elapsed_Time = Elapsed_Time + 100 - val_time;
                //??t l?i v? trí m?i ??i t??ng sau khi restart
                p_threat1.set_y_pos(-10);
                p_threat1.set_x_pos(rand() % SCREEN_WIDTH - 100);

                p_threat2.set_y_pos(-10);
                p_threat2.set_x_pos(rand() % SCREEN_WIDTH - 100);

                p_threat3.set_y_pos(-10);
                p_threat3.set_x_pos(rand() % SCREEN_WIDTH - 100);

                p_threat4.set_y_pos(-10);
                p_threat4.set_x_pos(rand() % SCREEN_WIDTH - 100);

                p_threat5.set_y_pos(-10);
                p_threat5.set_x_pos(rand() % SCREEN_WIDTH - 100);

                p_threat6.set_y_pos(-10);
                p_threat6.set_x_pos(rand() % SCREEN_WIDTH - 100);

                coin.set_x_pos(rand() % (SCREEN_WIDTH + 500));
                coin.set_y_pos(-10);

                power_fish.set_x_pos(rand() % (SCREEN_WIDTH + 500));
                power_fish.set_y_pos(-10);

                p_player.set_x_pos(200);
                p_player.set_y_pos(480);
                restart = false;
            }
            if (Time_inc) {
                x_time += time_inc_val;
                Time_inc = false;
            }
            if (val_time <= 0)
            {
                if (val_time <= 0)
                {
                    g_background.LoadImg("resource//Cat_PNG//youlose.png", g_screen);
                    die = true;
                    Mix_PlayChannel(-1, lose_sound, 0);
                    lose_sound = NULL;
                    Mix_HaltMusic();
                    time_game.Free();
                    mark_game.Free();
                    hp_game.Free();
                }
            }
            else if (!die)
            {
                std::string str_val = std::to_string(val_time);
                str_time += str_val;

                time_game.SetText(str_time);
                time_game.LoadFromRenderText(font_time, g_screen);
                time_game.RenderText(g_screen, SCREEN_WIDTH - 200, 15);
            }
            //Tip, Mark, HP text
            std::string val_str_mark = std::to_string(mark_value);
            std::string strMark("Score: ");
            strMark += val_str_mark;

            std::string strHp("/10");
            std::string str_power_left = std::to_string(hp_left) + strHp;

            std::string home("PLAY AGAIN");
            std::string exit("EXIT GAME");
            if (!die)
            {

                mark_game.SetText(strMark);
                mark_game.LoadFromRenderText(font_time, g_screen);
                mark_game.RenderText(g_screen, SCREEN_WIDTH * 0.5 - 60, 15);

                hp_game.SetText(str_power_left);
                hp_game.LoadFromRenderText(font_time, g_screen);
                hp_game.RenderText(g_screen, 12, 20);

                std::string continue_tex = "*Tip: press 'Enter' to pause/continue";
                continue_text.SetText(continue_tex);
                continue_text.LoadFromRenderText(font_time, g_screen);
                continue_text.RenderText(g_screen, SCREEN_WIDTH - 650, 80);
            }
            else if (die)
            {
                mark_die.SetText(strMark);
                mark_die.LoadFromRenderText(font_time, g_screen);
                mark_die.RenderText(g_screen, SCREEN_WIDTH * 0.5 - 60, 195);

                home_text.SetText(home);
                home_text.LoadFromRenderText(font_time, g_screen);
                if (current_end_menu == 1) {
                    home_text.SetColor(TextObject::RED_TEXT);
                }
                else home_text.SetColor(TextObject::WHITE_TEXT);
                home_text.RenderText(g_screen, 100, 395);

                exit2_text.SetText(exit);
                exit2_text.LoadFromRenderText(font_time, g_screen);
                if (current_end_menu == 2) {
                    exit2_text.SetColor(TextObject::RED_TEXT);
                }
                else exit2_text.SetColor(TextObject::WHITE_TEXT);
                exit2_text.RenderText(g_screen, SCREEN_WIDTH - 300, 395);

            }
        }
        if (GAME_STATUS == STATUS_FIRST_MENU)
        {
            play_text.SetText("PLAY GAME");
            play_text.LoadFromRenderText(font_time, g_screen);
            if (current_menu == 1) play_text.SetColor(TextObject::RED_TEXT);
            else play_text.SetColor(TextObject::BLACK_TEXT);
            play_text.RenderText(g_screen, SCREEN_WIDTH * 0.5 - 95, 350);


            tutorials_text.SetText("TUTORIALS");
            tutorials_text.LoadFromRenderText(font_time, g_screen);
            if (current_menu == 2) tutorials_text.SetColor(TextObject::RED_TEXT);
            else  tutorials_text.SetColor(TextObject::BLACK_TEXT);
            tutorials_text.RenderText(g_screen, SCREEN_WIDTH * 0.5 - 95, 450);

            exit1_text.SetText("EXIT GAME");
            exit1_text.LoadFromRenderText(font_time, g_screen);
            if (current_menu == 3) exit1_text.SetColor(TextObject::RED_TEXT);
            else  exit1_text.SetColor(TextObject::BLACK_TEXT);
            exit1_text.RenderText(g_screen, SCREEN_WIDTH * 0.5 - 95, 550);
        }
        //FPS           
        SDL_RenderPresent(g_screen);
        int fps_act;
        if (p_player.check_act() == true)
        {
            fps_act = FRAME_PER_SECOND;
        }
        else
        {
            fps_act = wait_FRAME_PER_SECOND;

        }
        int real_imp_time = fps_timer.get_ticks();
        int time_one_frame = 1000 / fps_act;

        if (real_imp_time < time_one_frame)
        {
            int delay_time = time_one_frame - real_imp_time;
            if (delay_time >= 0)
                SDL_Delay(delay_time);
        }
    }

    close();
    return 0;
}