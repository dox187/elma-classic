#include "all.h"
#include <SDL2/SDL.h>
#include <dirent.h>
#include <array>
#include <chrono>
#include <deque>
#include <string>
#include <vector>

#undef fopen
#undef access

int Win_95 = 1;
int Margrafikus = 0;
int Locky0_alul = 0;

namespace {
SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
SDL_Texture* texture = NULL;
std::array<unsigned char, 640 * 480> pixels;
std::array<unsigned char*, 480> rows;
std::array<uint32_t, 640 * 480> rgba;
std::array<uint32_t, 256> palette;
std::array<BYTE, 256> keys;
std::deque<int> chars;
bool locked = false;
bool running = true;
std::chrono::steady_clock::time_point stopper_start;

int dik(SDL_Scancode code) {
    switch (code) {
    case SDL_SCANCODE_ESCAPE: return DIK_ESCAPE;
    case SDL_SCANCODE_1: return DIK_1; case SDL_SCANCODE_2: return DIK_2;
    case SDL_SCANCODE_3: return DIK_3; case SDL_SCANCODE_4: return DIK_4;
    case SDL_SCANCODE_5: return DIK_5; case SDL_SCANCODE_6: return DIK_6;
    case SDL_SCANCODE_7: return DIK_7; case SDL_SCANCODE_8: return DIK_8;
    case SDL_SCANCODE_9: return DIK_9; case SDL_SCANCODE_0: return DIK_0;
    case SDL_SCANCODE_A: return DIK_A; case SDL_SCANCODE_B: return DIK_B;
    case SDL_SCANCODE_C: return DIK_C; case SDL_SCANCODE_D: return DIK_D;
    case SDL_SCANCODE_E: return DIK_E; case SDL_SCANCODE_F: return DIK_F;
    case SDL_SCANCODE_G: return DIK_G; case SDL_SCANCODE_H: return DIK_H;
    case SDL_SCANCODE_I: return DIK_I; case SDL_SCANCODE_J: return DIK_J;
    case SDL_SCANCODE_K: return DIK_K; case SDL_SCANCODE_L: return DIK_L;
    case SDL_SCANCODE_M: return DIK_M; case SDL_SCANCODE_N: return DIK_N;
    case SDL_SCANCODE_O: return DIK_O; case SDL_SCANCODE_P: return DIK_P;
    case SDL_SCANCODE_Q: return DIK_Q; case SDL_SCANCODE_R: return DIK_R;
    case SDL_SCANCODE_S: return DIK_S; case SDL_SCANCODE_T: return DIK_T;
    case SDL_SCANCODE_U: return DIK_U; case SDL_SCANCODE_V: return DIK_V;
    case SDL_SCANCODE_W: return DIK_W; case SDL_SCANCODE_X: return DIK_X;
    case SDL_SCANCODE_Y: return DIK_Y; case SDL_SCANCODE_Z: return DIK_Z;
    case SDL_SCANCODE_RETURN: return DIK_RETURN;
    case SDL_SCANCODE_KP_ENTER: return DIK_NUMPADENTER;
    case SDL_SCANCODE_BACKSPACE: return DIK_BACK;
    case SDL_SCANCODE_TAB: return DIK_TAB;
    case SDL_SCANCODE_SPACE: return DIK_SPACE;
    case SDL_SCANCODE_MINUS: return DIK_MINUS;
    case SDL_SCANCODE_EQUALS: return DIK_EQUALS;
    case SDL_SCANCODE_PERIOD: return DIK_PERIOD;
    case SDL_SCANCODE_COMMA: return DIK_COMMA;
    case SDL_SCANCODE_UP: return DIK_UP; case SDL_SCANCODE_DOWN: return DIK_DOWN;
    case SDL_SCANCODE_LEFT: return DIK_LEFT; case SDL_SCANCODE_RIGHT: return DIK_RIGHT;
    case SDL_SCANCODE_PAGEUP: return DIK_PRIOR; case SDL_SCANCODE_PAGEDOWN: return DIK_NEXT;
    case SDL_SCANCODE_DELETE: return DIK_DELETE;
    case SDL_SCANCODE_LCTRL: return DIK_LCONTROL; case SDL_SCANCODE_RCTRL: return DIK_RCONTROL;
    case SDL_SCANCODE_LSHIFT: return DIK_LSHIFT; case SDL_SCANCODE_RSHIFT: return DIK_RSHIFT;
    case SDL_SCANCODE_LALT: return DIK_LMENU; case SDL_SCANCODE_RALT: return DIK_RMENU;
    case SDL_SCANCODE_KP_0: return DIK_NUMPAD0; case SDL_SCANCODE_KP_1: return DIK_NUMPAD1;
    case SDL_SCANCODE_KP_2: return DIK_NUMPAD2; case SDL_SCANCODE_KP_3: return DIK_NUMPAD3;
    case SDL_SCANCODE_KP_4: return DIK_NUMPAD4; case SDL_SCANCODE_KP_5: return DIK_NUMPAD5;
    case SDL_SCANCODE_KP_6: return DIK_NUMPAD6; case SDL_SCANCODE_KP_7: return DIK_NUMPAD7;
    case SDL_SCANCODE_KP_8: return DIK_NUMPAD8; case SDL_SCANCODE_KP_9: return DIK_NUMPAD9;
    default: return 0;
    }
}

int character(const SDL_KeyboardEvent& event) {
    const SDL_Keycode key = event.keysym.sym;
    if (key >= SDLK_a && key <= SDLK_z)
        return (event.keysym.mod & KMOD_SHIFT) ? key - SDLK_a + 'A' : key;
    if (key >= SDLK_0 && key <= SDLK_9) return key;
    switch (key) {
    case SDLK_ESCAPE: return MK_ESC; case SDLK_RETURN: return MK_ENTER;
    case SDLK_UP: return MK_UP; case SDLK_DOWN: return MK_DOWN;
    case SDLK_LEFT: return MK_LEFT; case SDLK_RIGHT: return MK_RIGHT;
    case SDLK_PAGEUP: return MK_PGUP; case SDLK_PAGEDOWN: return MK_PGDOWN;
    case SDLK_DELETE: return MK_DEL; case SDLK_BACKSPACE: return MK_BACKSPACE;
    case SDLK_SPACE: return ' '; case SDLK_MINUS: return '-'; case SDLK_PERIOD: return '.';
    default: return 0;
    }
}

void events() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) running = false;
        if (event.type == SDL_KEYDOWN || event.type == SDL_KEYUP) {
            const int code = dik(event.key.keysym.scancode);
            if (code) keys[code] = event.type == SDL_KEYDOWN ? 0x80 : 0;
            if (event.type == SDL_KEYDOWN && !event.key.repeat) {
                const int c = character(event.key);
                if (c) chars.push_back(c);
            }
        }
    }
    if (!running) mv_exit();
}

void present() {
    for (size_t i = 0; i < pixels.size(); ++i) rgba[i] = palette[pixels[i]];
    SDL_UpdateTexture(texture, NULL, rgba.data(), 640 * sizeof(uint32_t));
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);
}
}

static std::string join_path(const std::string& dir, const std::string& name) {
    if (dir.empty()) return name;
    if (dir[dir.size() - 1] == '/') return dir + name;
    return dir + "/" + name;
}

// The game names its files case-insensitively (elma.res, lgr\default.lgr),
// as Windows allows. On case-sensitive file systems, match each missing path
// component to an existing entry ignoring case; unmatched names are kept so
// new files can still be created.
static std::string native_path(const char* path) {
    std::string result = path;
    for (size_t i = 0; i < result.size(); ++i)
        if (result[i] == '\\') result[i] = '/';
    if (result.empty() || access(result.c_str(), F_OK) == 0) return result;

    std::string resolved = result[0] == '/' ? "/" : "";
    size_t start = resolved.size();
    while (start < result.size()) {
        size_t end = result.find('/', start);
        if (end == std::string::npos) end = result.size();
        const std::string name = result.substr(start, end - start);
        std::string candidate = join_path(resolved, name);
        if (!name.empty() && access(candidate.c_str(), F_OK) != 0) {
            if (DIR* dir = opendir(resolved.empty() ? "." : resolved.c_str())) {
                while (dirent* entry = readdir(dir)) {
                    if (strcasecmp(entry->d_name, name.c_str()) == 0) {
                        candidate = join_path(resolved, entry->d_name);
                        break;
                    }
                }
                closedir(dir);
            }
        }
        resolved = candidate;
        start = end + 1;
    }
    return resolved;
}

FILE* elma_fopen(const char* path, const char* mode) {
    return fopen(native_path(path).c_str(), mode);
}

int elma_access(const char* path, int mode) {
    return access(native_path(path).c_str(), mode);
}

void ddinit() {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_TIMER) != 0) {
        fprintf(stderr, "SDL_Init: %s\n", SDL_GetError());
        exit(1);
    }
    window = SDL_CreateWindow("Elasto Mania", SDL_WINDOWPOS_CENTERED,
                              SDL_WINDOWPOS_CENTERED, 640, 480,
                              SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_RESIZABLE);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer) renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888,
                                SDL_TEXTUREACCESS_STREAMING, 640, 480);
    SDL_RenderSetLogicalSize(renderer, 640, 480);
    Margrafikus = 1;
}

void ddfinish() {
    if (texture) SDL_DestroyTexture(texture);
    if (renderer) SDL_DestroyRenderer(renderer);
    if (window) SDL_DestroyWindow(window);
    texture = NULL; renderer = NULL; window = NULL;
    SDL_Quit();
}

unsigned char** lockbackbuffer(int) {
    if (locked) hiba((char*)"framebuffer already locked");
    locked = true;
    for (int y = 0; y < 480; ++y)
        rows[Locky0_alul ? 479 - y : y] = pixels.data() + y * 640;
    return rows.data();
}

void unlockbackbuffer() { locked = false; present(); }
unsigned char** lockfrontbuffer() { return lockbackbuffer(0); }
void unlockfrontbuffer() { unlockbackbuffer(); }

ddpal::ddpal(unsigned char* colors) {
    std::array<uint32_t, 256>* value = new std::array<uint32_t, 256>;
    for (int i = 0; i < 256; ++i) {
        const uint32_t r = colors[i * 3] << 2;
        const uint32_t g = colors[i * 3 + 1] << 2;
        const uint32_t b = colors[i * 3 + 2] << 2;
        (*value)[i] = 0xff000000u | r << 16 | g << 8 | b;
    }
    pddpal = value;
}
ddpal::~ddpal() { delete static_cast<std::array<uint32_t, 256>*>(pddpal); }
void ddpal::set() { palette = *static_cast<std::array<uint32_t, 256>*>(pddpal); }

void mk_init() { keys.fill(0); chars.clear(); }
int mk_getextchar() { while (chars.empty()) { mv_check(); SDL_Delay(1); } int c = chars.front(); chars.pop_front(); return c; }
void mk_emptychar() { events(); chars.clear(); }
int mk_kbhit() { events(); return !chars.empty(); }
int mk_getstate(int code) { events(); return code >= 0 && code < 256 ? keys[code] : 0; }
int controlaltnyomva() { return keys[DIK_LMENU] && keys[DIK_LCONTROL]; }

void di_reset_state(state_s* state) {
    state->opciok1.billgaz = DIK_UP; state->opciok1.billfek = DIK_DOWN;
    state->opciok1.billugras1 = DIK_RIGHT; state->opciok1.billugras2 = DIK_LEFT;
    state->opciok1.billfordul = DIK_SPACE; state->opciok1.billview = DIK_V;
    state->opciok1.billtime = DIK_T; state->opciok1.billshowkep = DIK_1;
    state->opciok2.billgaz = DIK_NUMPAD5; state->opciok2.billfek = DIK_NUMPAD2;
    state->opciok2.billugras1 = DIK_NUMPAD3; state->opciok2.billugras2 = DIK_NUMPAD1;
    state->opciok2.billfordul = DIK_NUMPAD0; state->opciok2.billview = DIK_B;
    state->opciok2.billtime = DIK_Y; state->opciok2.billshowkep = DIK_2;
    state->billplussz = DIK_EQUALS; state->billminusz = DIK_MINUS; state->billsnap = DIK_I;
}

char* kodtobillnev(int code) {
    static char name[32];
    snprintf(name, sizeof(name), "KEY %02X", code);
    return name;
}

void mv_check(int) { events(); }
void mv_startstopper() { stopper_start = std::chrono::steady_clock::now(); }
double mv_stopperido() {
    const std::chrono::duration<double> elapsed = std::chrono::steady_clock::now() - stopper_start;
    return elapsed.count() * 182.0;
}
void getbiostort(unsigned short* bios, unsigned short* fraction) {
    const uint64_t ticks = SDL_GetPerformanceCounter();
    *bios = static_cast<unsigned short>(ticks >> 16);
    *fraction = static_cast<unsigned short>(ticks);
}
int s_random(int max) { return max > 0 ? rand() % max : 0; }

void getmou(int* x, int* y) { SDL_GetMouseState(x, y); }
void setmou(int x, int y) { SDL_WarpMouseInWindow(window, x, y); }
int getbutbmou() { return SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_LEFT); }
int getbutjmou() { return SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_RIGHT); }

static std::string message(char* a, char* b, char* c) {
    std::string text = a ? a : "";
    if (b) text += std::string("\n") + b;
    if (c) text += std::string("\n") + c;
    return text;
}
void hiba(char* a, char* b, char* c) {
    const std::string text = message(a, b, c);
    fprintf(stderr, "%s\n", text.c_str());
    if (window) SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Elasto Mania", text.c_str(), window);
    ddfinish(); exit(1);
}
void uzenet(char* a, char* b, char* c) {
    const std::string text = message(a, b, c);
    if (window) SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Elasto Mania", text.c_str(), window);
    else fprintf(stderr, "%s\n", text.c_str());
}
void mv_exit(char*) { ddfinish(); exit(0); }

int main(int, char**) {
    void upgradehakell(); void initqopen();
    void initmenukep1(); void initmenukep2();
    upgradehakell(); initqopen(); mk_init(); initmenukep1();
    ddinit(); initmenukep2(); mv_main();
    return 0;
}
