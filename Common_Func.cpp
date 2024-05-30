

#include "Common_Func.h"

//BEGIN BASE OBJECT

BaseObject::BaseObject() {
    rect_.x = 0;
    rect_.y = 0;
    rect_.w = 0;
    rect_.h = 0;
    p_object_ = NULL;
}
BaseObject::~BaseObject() {
    Free();
}
bool BaseObject::LoadImg(std::string path, SDL_Renderer* screen)
{
    SDL_Texture* new_texture = NULL;

    SDL_Surface* load_surface = IMG_Load(path.c_str());
    if (load_surface != NULL)
    {
        SDL_SetColorKey(load_surface, SDL_TRUE, SDL_MapRGB(load_surface->format, COLOR_KEY_R, COLOR_KEY_G, COLOR_KEY_B));
        new_texture = SDL_CreateTextureFromSurface(screen, load_surface);
        if (new_texture != NULL)
        {
            rect_.w = load_surface->w;
            rect_.h = load_surface->h;
        }
        SDL_FreeSurface(load_surface);
    }
    p_object_ = new_texture;

    if (p_object_ != NULL) return true;
    return false;
}

void BaseObject::Render(SDL_Renderer* des, const SDL_Rect* clip)
{
    SDL_Rect renderquad = { rect_.x, rect_.y, rect_.w, rect_.h };

    SDL_RenderCopy(des, p_object_, clip, &renderquad);

}

void BaseObject::Free()
{
    if (p_object_ != NULL)
    {
        SDL_DestroyTexture(p_object_);
        p_object_ = NULL;
        rect_.w = 0;
        rect_.h = 0;

    }
}

// END BASE OBJECT

// BEGIN MAIN OBJECT

MainObject::MainObject() {
    frame_ = 0;
    x_pos_ = 200;
    y_pos_ = 480;
    x_val_ = 0;
    y_val_ = 0;
    width_frame_ = 0;
    height_frame_ = 0;
    status_ = -1;
    input_type_.left_ = 0;
    input_type_.right_ = 0;
    input_type_.jump_ = 0;
    input_type_.down_ = 0;
    input_type_.up_ = 0;
    player_speed = 1.7;
}
MainObject::~MainObject() {
    p_object_ = NULL;
}

bool MainObject::LoadImg(std::string path, SDL_Renderer* screen)
{
    bool ret = BaseObject::LoadImg(path, screen);

    if (ret == true)
    {
        width_frame_ = rect_.w / 6;
        height_frame_ = rect_.h;
    }
    return ret;
}

void MainObject::set_clips()
{
    if (width_frame_ > 0 && height_frame_ > 0)
    {
        frame_clip_[0].x = 0;
        frame_clip_[0].y = 0;
        frame_clip_[0].w = width_frame_;
        frame_clip_[0].h = height_frame_;

        frame_clip_[1].x = width_frame_;
        frame_clip_[1].y = 0;
        frame_clip_[1].w = width_frame_;
        frame_clip_[1].h = height_frame_;

        frame_clip_[2].x = 2 * width_frame_;
        frame_clip_[2].y = 0;
        frame_clip_[2].w = width_frame_;
        frame_clip_[2].h = height_frame_;

        frame_clip_[3].x = 3 * width_frame_;
        frame_clip_[3].y = 0;
        frame_clip_[3].w = width_frame_;
        frame_clip_[3].h = height_frame_;

        frame_clip_[4].x = 4 * width_frame_;
        frame_clip_[4].y = 0;
        frame_clip_[4].w = width_frame_;
        frame_clip_[4].h = height_frame_;

        frame_clip_[5].x = 5 * width_frame_;
        frame_clip_[5].y = 0;
        frame_clip_[5].w = width_frame_;
        frame_clip_[5].h = height_frame_;
    }
}

void MainObject::Show(SDL_Renderer* des)
{
    if (status_ == WALK_LEFT)
    {
        LoadImg("resource//Cat_PNG//walk_sau.png", des);

    }
    else if (status_ == WALK_RIGHT)
    {
        LoadImg("resource//Cat_PNG//walk_trc.png", des);
    }
    else if (status_ == WAIT_RIGHT)
    {

        LoadImg("resource//Cat_PNG//wait_trc.png", des);
        if (input_type_.left_ == 0 ||
            input_type_.right_ == 0)
        {
            frame_++;
        }
        else
        {
            frame_ = 0;
        }

        if (frame_ >= 6)
        {
            frame_ = 0;
        }
    }
    else
    {
        LoadImg("resource//Cat_PNG//wait_sau.png", des);
        if (input_type_.left_ == 0 ||
            input_type_.right_ == 0)
        {
            frame_++;
        }
        else
        {
            frame_ = 0;
        }

        if (frame_ >= 6)
        {
            frame_ = 0;
        }

    }
    if (input_type_.left_ == 1 ||
        input_type_.right_ == 1)
    {
        frame_++;
    }
    if (frame_ >= 6)
    {
        frame_ = 0;
    }
    rect_.x = x_pos_;
    rect_.y = y_pos_;

    SDL_Rect* current_clip = &frame_clip_[frame_];

    SDL_Rect renderQuad = { rect_.x, rect_.y, width_frame_, height_frame_ };

    SDL_RenderCopy(des, p_object_, current_clip, &renderQuad);
}

void MainObject::HandleInputAction(SDL_Event events, SDL_Renderer* screen) {

    x_val_ = player_speed * width_frame_ / 6;
    if (events.type == SDL_KEYDOWN)
    {
        switch (events.key.keysym.sym)
        {
        case SDLK_RIGHT:
        {
            status_ = WALK_RIGHT;
            input_type_.right_ = 1;
            input_type_.left_ = 0;
            if (x_pos_ >= SCREEN_WIDTH - width_frame_)
            {
                x_val_ = 0;
                x_pos_ = SCREEN_WIDTH - width_frame_;
            }
            x_pos_ += x_val_;
        }
        break;
        case SDLK_LEFT:
        {
            status_ = WALK_LEFT;
            input_type_.left_ = 1;
            input_type_.right_ = 0;
            if (x_pos_ <= 0 - width_frame_ / 9) x_val_ = 0;
            x_pos_ -= x_val_;
        }
        break;
        case SDLK_a:
        {
            status_ = WALK_LEFT;
            input_type_.left_ = 1;
            input_type_.right_ = 0;
            if (x_pos_ <= width_frame_)
            {
                x_val_ = 0;
                x_pos_ = 0;
            }
            x_pos_ -= 3 * x_val_;
        }
        break;
        case SDLK_d:
        {
            status_ = WALK_RIGHT;
            input_type_.left_ = 0;
            input_type_.right_ = 1;
            if (x_pos_ >= SCREEN_WIDTH - 2 * width_frame_)
            {
                x_val_ = 0;
                x_pos_ = SCREEN_WIDTH - width_frame_;
            }
            x_pos_ += 3 * x_val_;
        }
        break;
        }
    }
    else if (events.type == SDL_KEYUP)
    {
        switch (events.key.keysym.sym)
        {
        case SDLK_RIGHT:
        {
            input_type_.right_ = 0;
            status_ = WAIT_RIGHT;
        }
        break;
        case SDLK_LEFT:
        {
            input_type_.left_ = 0;
            status_ = WAIT_LEFT;
        }
        break;
        case SDLK_a:
        {
            input_type_.left_ = 0;
            status_ = WAIT_LEFT;
        }
        break;
        case SDLK_d:
        {
            input_type_.right_ = 0;
            status_ = WAIT_RIGHT;
        }
        break;
        }
    }
}

bool MainObject::check_act()
{
    if (status_ == WALK_LEFT || status_ == WALK_RIGHT)
        return true;
    return false;
}

// END MAIN OBJECT

//TIMER
ImpTimer::ImpTimer()
{
    start_tick_ = 0;
    paused_tick_ = 0;
    is_paused_ = false;
    is_started_ = false;
}
ImpTimer::~ImpTimer()
{

}

void ImpTimer::start()
{
    is_started_ = true;
    is_paused_ = false;
    start_tick_ = SDL_GetTicks();
}
void ImpTimer::stop()
{
    is_paused_ = false;
    is_started_ = false;
}
void ImpTimer::paused()
{
    if (is_started_ == true && is_paused_ == false)
    {
        is_paused_ = true;
        paused_tick_ = SDL_GetTicks() - start_tick_;
    }
}
void ImpTimer::unpaused()
{
    if (is_paused_ == true)
    {
        is_paused_ = false;
        start_tick_ = SDL_GetTicks() - paused_tick_;
        paused_tick_ = 0;
    }
}

int ImpTimer::get_ticks()
{
    if (is_started_ == true)
    {
        if (is_paused_ == true)
        {
            return paused_tick_;
        }
        else
        {
            return SDL_GetTicks() - start_tick_;
        }
    }
    return 0;
}

bool ImpTimer::is_started()
{
    return is_started_;
}
bool ImpTimer::is_paused()
{
    return is_paused_;
}

//END TIMER

//BEGIN THREATS
ThreatsObject::ThreatsObject()
{
    p_object_ = NULL;
    rect_.x = 0;
    rect_.y = 0;
    width_frame_ = 0;
    height_frame_ = 0;
    x_val_ = 0.0;
    y_val_ = 0.0;
    x_pos_ = 0.0;
    y_pos_ = 0.0;
    on_ground_ = 0;
    come_back_time = 0;
    frame_ = 0;
    drop_speed = 40;
}
ThreatsObject::~ThreatsObject()
{

}
bool ThreatsObject::_BaseObject_LoadImg(std::string path, SDL_Renderer* screen, int width, int height)
{
    // Đọc ảnh với kênh alpha bằng OpenCV
    cv::Mat image = cv::imread(path, cv::IMREAD_UNCHANGED);
    if (image.empty()) {
        std::cout << "Could not open or find the image" << std::endl;
        return false;
    }

    // Thay đổi kích thước ảnh
    cv::resize(image, image, cv::Size(width, height));

    // Chuyển đổi từ BGR hoặc BGRA sang RGBA (SDL sử dụng RGBA)
    if (image.channels() == 3) {
        cv::cvtColor(image, image, cv::COLOR_BGR2RGB);
    }
    else if (image.channels() == 4) {
        cv::cvtColor(image, image, cv::COLOR_BGRA2RGBA);
    }

    // Tạo SDL_Surface từ dữ liệu của cv::Mat
    SDL_Surface* surface = SDL_CreateRGBSurfaceFrom(
        image.data,
        image.cols,
        image.rows,
        image.channels() * 8, // Bit depth per pixel
        image.step,
        0x000000FF,
        0x0000FF00,
        0x00FF0000,
        (image.channels() == 4) ? 0xFF000000 : 0
    );

    if (surface == nullptr) {
        std::cout << "SDL_CreateRGBSurfaceFrom failed: " << SDL_GetError() << std::endl;
        return false;
    }

    // Tạo SDL_Texture từ SDL_Surface
    SDL_Texture* new_texture = SDL_CreateTextureFromSurface(screen, surface);
    SDL_FreeSurface(surface); // Giải phóng surface sau khi tạo texture

    if (new_texture != nullptr) {
        rect_.w = width;
        rect_.h = height;
    }

    p_object_ = new_texture;

    return p_object_ != nullptr;
}
bool ThreatsObject::LoadImg(std::string path, SDL_Renderer* screen, int width, int height)
{
    bool ret = ThreatsObject::_BaseObject_LoadImg(path, screen, width, height);

    if (ret)
    {
        width_frame_ = rect_.w / 4;
        height_frame_ = rect_.h;
    }
    if (ret) return true;
    else return false;
}

void ThreatsObject::set_clips()
{
    if (width_frame_ > 0 && height_frame_ > 0)
    {
        frame_clip_[0].x = 0;
        frame_clip_[0].y = 0;
        frame_clip_[0].w = width_frame_;
        frame_clip_[0].h = height_frame_;

        frame_clip_[1].x = width_frame_;
        frame_clip_[1].y = 0;
        frame_clip_[1].w = width_frame_;
        frame_clip_[1].h = height_frame_;

        frame_clip_[2].x = 2 * width_frame_;
        frame_clip_[2].y = 0;
        frame_clip_[2].w = width_frame_;
        frame_clip_[2].h = height_frame_;

        frame_clip_[3].x = 3 * width_frame_;
        frame_clip_[3].y = 0;
        frame_clip_[3].w = width_frame_;
        frame_clip_[3].h = height_frame_;
    }
}

void ThreatsObject::Show(SDL_Renderer* des)
{
    LoadImg("resource//Cat_PNG//ball.png", des, 323, 79);

    rect_.x = x_pos_;// - map_x_;
    rect_.y = y_pos_ += drop_speed;//- map_y_;
    if (y_pos_ >= 600)
    {
        y_pos_ = 0;
        x_pos_ = rand() % (SCREEN_WIDTH - width_frame_);
    }
    frame_++;
    if (frame_ >= 4)
    {
        frame_ = 0;
    }
    SDL_Rect* currentClip = &frame_clip_[frame_];
    SDL_Rect renderQuad = { rect_.x, rect_.y, width_frame_, height_frame_ };
    SDL_RenderCopy(des, p_object_, currentClip, &renderQuad);
}

//END THREAT

//BEGIN COIN

Coin_Item::Coin_Item()
{
    p_object_ = NULL;
    rect_.x = 0;
    rect_.y = 0;
    width_frame_ = 0;
    height_frame_ = 0;
    x_val_ = 0.0;
    y_val_ = 0.0;
    x_pos_ = 0.0;
    y_pos_ = 0.0;
    on_ground_ = 0;
    come_back_time = 0;
    frame_ = 0;
    drop_speed = 40;
}
Coin_Item::~Coin_Item()
{

}
bool Coin_Item::_BaseObject_LoadImg(std::string path, SDL_Renderer* screen, int width, int height)
{
    // Đọc ảnh với kênh alpha bằng OpenCV
    cv::Mat image = cv::imread(path, cv::IMREAD_UNCHANGED);
    if (image.empty()) {
        std::cout << "Could not open or find the image" << std::endl;
        return false;
    }

    // Thay đổi kích thước ảnh
    cv::resize(image, image, cv::Size(width, height));

    // Chuyển đổi từ BGR hoặc BGRA sang RGBA (SDL sử dụng RGBA)
    if (image.channels() == 3) {
        cv::cvtColor(image, image, cv::COLOR_BGR2RGB);
    }
    else if (image.channels() == 4) {
        cv::cvtColor(image, image, cv::COLOR_BGRA2RGBA);
    }

    // Tạo SDL_Surface từ dữ liệu của cv::Mat
    SDL_Surface* surface = SDL_CreateRGBSurfaceFrom(
        image.data,
        image.cols,
        image.rows,
        image.channels() * 8, // Bit depth per pixel
        image.step,
        0x000000FF,
        0x0000FF00,
        0x00FF0000,
        (image.channels() == 4) ? 0xFF000000 : 0
    );

    if (surface == nullptr) {
        std::cout << "SDL_CreateRGBSurfaceFrom failed: " << SDL_GetError() << std::endl;
        return false;
    }

    // Tạo SDL_Texture từ SDL_Surface
    SDL_Texture* new_texture = SDL_CreateTextureFromSurface(screen, surface);
    SDL_FreeSurface(surface); // Giải phóng surface sau khi tạo texture

    if (new_texture != nullptr) {
        rect_.w = width;
        rect_.h = height;
    }

    p_object_ = new_texture;

    return p_object_ != nullptr;
}
bool Coin_Item::LoadImg(std::string path, SDL_Renderer* screen, int width, int height)
{
    bool ret = Coin_Item::_BaseObject_LoadImg(path, screen, width, height);

    if (ret)
    {
        width_frame_ = rect_.w / 10;
        height_frame_ = rect_.h;
    }
    if (ret) return true;
    else return false;
}

void Coin_Item::set_clips()
{
    if (width_frame_ > 0 && height_frame_ > 0)
    {
        frame_clip_[0].x = 0;
        frame_clip_[0].y = 0;
        frame_clip_[0].w = width_frame_;
        frame_clip_[0].h = height_frame_;

        frame_clip_[1].x = width_frame_;
        frame_clip_[1].y = 0;
        frame_clip_[1].w = width_frame_;
        frame_clip_[1].h = height_frame_;

        frame_clip_[2].x = 2 * width_frame_;
        frame_clip_[2].y = 0;
        frame_clip_[2].w = width_frame_;
        frame_clip_[2].h = height_frame_;

        frame_clip_[3].x = 3 * width_frame_;
        frame_clip_[3].y = 0;
        frame_clip_[3].w = width_frame_;
        frame_clip_[3].h = height_frame_;

        frame_clip_[4].x = 4 * width_frame_;
        frame_clip_[4].y = 0;
        frame_clip_[4].w = width_frame_;
        frame_clip_[4].h = height_frame_;

        frame_clip_[5].x = 5 * width_frame_;
        frame_clip_[5].y = 0;
        frame_clip_[5].w = width_frame_;
        frame_clip_[5].h = height_frame_;

        frame_clip_[6].x = 6 * width_frame_;
        frame_clip_[6].y = 0;
        frame_clip_[6].w = width_frame_;
        frame_clip_[6].h = height_frame_;

        frame_clip_[7].x = 7 * width_frame_;
        frame_clip_[7].y = 0;
        frame_clip_[7].w = width_frame_;
        frame_clip_[7].h = height_frame_;

        frame_clip_[8].x = 8 * width_frame_;
        frame_clip_[8].y = 0;
        frame_clip_[8].w = width_frame_;
        frame_clip_[8].h = height_frame_;

        frame_clip_[9].x = 9 * width_frame_;
        frame_clip_[9].y = 0;
        frame_clip_[9].w = width_frame_;
        frame_clip_[9].h = height_frame_;
    }
}

void Coin_Item::Show(SDL_Renderer* des)
{
    if (type_of_coin <= 2)LoadImg("resource//Cat_PNG//G_coin.png", des, 700, 70);
    else if (type_of_coin <= 15 && type_of_coin > 2) LoadImg("resource//Cat_PNG//Sil_coin.png", des, 700, 70);
    else if (type_of_coin > 15 && type_of_coin <= 40) LoadImg("resource//Cat_PNG//Br_coin.png", des, 700, 70);
    else if (type_of_coin > 40) LoadImg("resource//Cat_PNG//G_star.png", des, 700, 70);


    rect_.x = x_pos_;// - map_x_;
    rect_.y = y_pos_ += drop_speed;//- map_y_;
    if (y_pos_ >= 600)
    {
        if (type_of_coin <= 2) type_of_coin = 20;

        y_pos_ = 0;
        x_pos_ = rand() % (SCREEN_WIDTH - width_frame_);
    }
    frame_++;
    if (frame_ >= 4)
    {
        frame_ = 0;
    }
    SDL_Rect* currentClip = &frame_clip_[frame_];
    SDL_Rect renderQuad = { rect_.x, rect_.y, width_frame_, height_frame_ };
    SDL_RenderCopy(des, p_object_, currentClip, &renderQuad);
}

//END_COIN


//BEGIN COMMONFUNC

bool SDLCommonFunc::CheckCollision(const SDL_Rect& object1, const SDL_Rect& object2)
{

    int left_a = object1.x;
    int right_a = object1.x + object1.w;
    int top_a = object1.y;
    int bottom_a = object1.y + object1.h;

    int left_b = object2.x;
    int right_b = object2.x + object2.w;
    int top_b = object2.y;
    int bottom_b = object2.y + object2.h;

    if (top_a <= (top_b - 10) && left_a <= left_b && right_a >= right_b) return true;
    else return false;
}

//END COMMONFUNC


//BEGIN ITEM OBJECT
ItemsObject::ItemsObject()
{
    p_object_ = NULL;
    rect_.x = 0;
    rect_.y = 0;
    width_frame_ = 0;
    height_frame_ = 0;
    x_val_ = 0.0;
    y_val_ = 0.0;
    x_pos_ = 0.0;
    y_pos_ = 0.0;
    on_ground_ = 0;
    come_back_time = 0;
    frame_ = 0;
    on_ground_ = false;
    drop_speed = 10;
}
ItemsObject::~ItemsObject()
{

}
bool ItemsObject::_BaseObject_LoadImg(std::string path, SDL_Renderer* screen, int width, int height)
{
    cv::Mat image = cv::imread(path, cv::IMREAD_UNCHANGED);
    if (image.empty()) {
        std::cout << "Could not open or find the image" << std::endl;
        return false;
    }

    // Thay đổi kích thước ảnh
    cv::resize(image, image, cv::Size(width, height));

    // Chuyển đổi từ BGR hoặc BGRA sang RGBA (SDL sử dụng RGBA)
    if (image.channels() == 3) {
        cv::cvtColor(image, image, cv::COLOR_BGR2RGB);
    }
    else if (image.channels() == 4) {
        cv::cvtColor(image, image, cv::COLOR_BGRA2RGBA);
    }

    // Tạo SDL_Surface từ dữ liệu của cv::Mat
    SDL_Surface* surface = SDL_CreateRGBSurfaceFrom(
        image.data,
        image.cols,
        image.rows,
        image.channels() * 8, // Bit depth per pixel
        image.step,
        0x000000FF,
        0x0000FF00,
        0x00FF0000,
        (image.channels() == 4) ? 0xFF000000 : 0
    );

    if (surface == nullptr) {
        std::cout << "SDL_CreateRGBSurfaceFrom failed: " << SDL_GetError() << std::endl;
        return false;
    }

    // Tạo SDL_Texture từ SDL_Surface
    SDL_Texture* new_texture = SDL_CreateTextureFromSurface(screen, surface);
    SDL_FreeSurface(surface); // Giải phóng surface sau khi tạo texture

    if (new_texture != nullptr) {
        rect_.w = width;
        rect_.h = height;
    }

    p_object_ = new_texture;

    return p_object_ != nullptr;
}
bool ItemsObject::LoadImg(std::string path, SDL_Renderer* screen, int width, int height)
{
    bool ret = ItemsObject::_BaseObject_LoadImg(path, screen, width, height);

    if (ret)
    {
        width_frame_ = rect_.w / 4;
        height_frame_ = rect_.h;
    }
    if (ret) return true;
    else return false;
}

void ItemsObject::set_clips()
{
    if (width_frame_ > 0 && height_frame_ > 0)
    {
        frame_clip_[0].x = 0;
        frame_clip_[0].y = 0;
        frame_clip_[0].w = width_frame_;
        frame_clip_[0].h = height_frame_;

        frame_clip_[1].x = width_frame_;
        frame_clip_[1].y = 0;
        frame_clip_[1].w = width_frame_;
        frame_clip_[1].h = height_frame_;

        frame_clip_[2].x = 2 * width_frame_;
        frame_clip_[2].y = 0;
        frame_clip_[2].w = width_frame_;
        frame_clip_[2].h = height_frame_;

        frame_clip_[3].x = 3 * width_frame_;
        frame_clip_[3].y = 0;
        frame_clip_[3].w = width_frame_;
        frame_clip_[3].h = height_frame_;
    }
}

void ItemsObject::Show(SDL_Renderer* des)
{
    LoadImg("resource//Cat_PNG//fish.png", des, 531, 84);

    rect_.x = x_pos_;// - map_x_;
    rect_.y = y_pos_ += drop_speed;//- map_y_;
    if (y_pos_ >= 600)
    {

        on_ground_ = true;
        y_pos_ = 0;
        x_pos_ = rand() % (SCREEN_WIDTH - width_frame_);

    }
    frame_++;
    if (frame_ >= 4)
    {
        frame_ = 0;
    }
    SDL_Rect* currentClip = &frame_clip_[frame_];
    SDL_Rect renderQuad = { rect_.x, rect_.y, width_frame_, height_frame_ };
    SDL_RenderCopy(des, p_object_, currentClip, &renderQuad);
}

//END PLAYERPOWER

//BEGIN TEXT

TextObject::TextObject()
{
    text_color_.r = 255;
    text_color_.g = 255;
    text_color_.b = 255;
    texture_ = NULL;
}

TextObject::~TextObject()
{

}

bool TextObject::LoadFromRenderText(TTF_Font* font, SDL_Renderer* screen)
{
    SDL_Surface* text_surface = TTF_RenderText_Solid(font, str_val_.c_str(), text_color_);
    if (text_surface)
    {
        texture_ = SDL_CreateTextureFromSurface(screen, text_surface);
        width_ = text_surface->w;
        height_ = text_surface->h;

        SDL_FreeSurface(text_surface);

    }
    return texture_ != NULL;

}

void TextObject::Free()
{
    if (texture_ != NULL)
    {
        SDL_DestroyTexture(texture_);
        texture_ = NULL;
    }
}

void TextObject::SetColor(Uint8 red, Uint8 green, Uint8 blue)
{
    text_color_.r = red;
    text_color_.g = green;
    text_color_.b = blue;
}

void TextObject::SetColor(int type)
{
    if (type == RED_TEXT)
    {
        SDL_Color color = { 255,0,0 };
        text_color_ = color;
    }
    else if (type == WHITE_TEXT)
    {
        SDL_Color color = { 255,255,255 };
        text_color_ = color;
    }
    else if (type == BLACK_TEXT)
    {
        SDL_Color color = { 0,0,0 };
        text_color_ = color;
    }
}

void TextObject::RenderText(SDL_Renderer* screen,
    int xp, int yp, SDL_Rect* clip,
    double angle, SDL_Point* center,
    SDL_RendererFlip flip)
{
    SDL_Rect renderQuad = { xp,yp, width_, height_ };
    if (clip != NULL)
    {
        renderQuad.w = clip->w;
        renderQuad.h = clip->h;

    }

    SDL_RenderCopyEx(screen, texture_, clip, &renderQuad, angle, center, flip);

}

//END TEXT

//BEGIN MENU OBJECT

MenuObject::MenuObject() {
    frame_ = 0;
    x_pos_ = 200;
    y_pos_ = 480;
    x_val_ = 0;
    y_val_ = 0;
    width_frame_ = 0;
    height_frame_ = 0;
    //status_ = -1;
}
MenuObject::~MenuObject() {
    p_object_ = NULL;
}

bool MenuObject::LoadImg(std::string path, SDL_Renderer* screen)
{
    bool ret = BaseObject::LoadImg(path, screen);

    if (ret == true)
    {
        width_frame_ = rect_.w / 8;
        height_frame_ = rect_.h;
    }
    return ret;
}

void MenuObject::set_clips()
{
    if (width_frame_ > 0 && height_frame_ > 0)
    {
        frame_clip_[0].x = 0;
        frame_clip_[0].y = 0;
        frame_clip_[0].w = width_frame_;
        frame_clip_[0].h = height_frame_;

        frame_clip_[1].x = width_frame_;
        frame_clip_[1].y = 0;
        frame_clip_[1].w = width_frame_;
        frame_clip_[1].h = height_frame_;

        frame_clip_[2].x = 2 * width_frame_;
        frame_clip_[2].y = 0;
        frame_clip_[2].w = width_frame_;
        frame_clip_[2].h = height_frame_;

        frame_clip_[3].x = 3 * width_frame_;
        frame_clip_[3].y = 0;
        frame_clip_[3].w = width_frame_;
        frame_clip_[3].h = height_frame_;

        frame_clip_[4].x = 4 * width_frame_;
        frame_clip_[4].y = 0;
        frame_clip_[4].w = width_frame_;
        frame_clip_[4].h = height_frame_;

        frame_clip_[5].x = 5 * width_frame_;
        frame_clip_[5].y = 0;
        frame_clip_[5].w = width_frame_;
        frame_clip_[5].h = height_frame_;

        frame_clip_[6].x = 6 * width_frame_;
        frame_clip_[6].y = 0;
        frame_clip_[6].w = width_frame_;
        frame_clip_[6].h = height_frame_;

        frame_clip_[7].x = 7 * width_frame_;
        frame_clip_[7].y = 0;
        frame_clip_[7].w = width_frame_;
        frame_clip_[7].h = height_frame_;
    }
}

void MenuObject::Show(SDL_Renderer* des)
{
    LoadImg("resource//Cat_PNG//menu_cat.png", des);
    frame_++;
    if (frame_ >= 8)
    {
        frame_ = 0;
    }

    rect_.x = x_pos_;
    rect_.y = y_pos_;

    SDL_Rect* current_clip = &frame_clip_[frame_];

    SDL_Rect renderQuad = { rect_.x, rect_.y, width_frame_, height_frame_ };

    SDL_RenderCopy(des, p_object_, current_clip, &renderQuad);
}
