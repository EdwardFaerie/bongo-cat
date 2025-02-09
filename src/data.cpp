#include "header.hpp"
#define BONGO_ERROR 1

#if defined(__unix__) || defined(__unix)
#include <unistd.h>
#include <limits.h>

extern "C" {
#include <SDL2/SDL.h>
}
#else
#include <windows.h>
#endif

namespace data {
Json::Value cfg;
std::map<std::string, sf::Texture> img_holder;

void create_config() {
    const char *s =
        R"V0G0N({
    "_Comment Modes & descriptions": [
        {
            "1": "Osu, The classic bongo cat with a keyboard and mouse.",
            "2": "taiko, classic instrument game.",
            "3": "ctb, catch game.",
            "4": "Mania, Mania games.",
            "5": "Custom, a custom mode.",
            "6": "Gamer, it has different settings, from unlimited customizable costumes, customizable background, customizable desk, mouth movement with microphone sound compatibility and many others."
        }
    ],
    "mode": 6,
    "resolution": {
        "letterboxing": false,
        "width": 1920,
        "height": 1080,
        "horizontalPosition": 0,
        "verticalPosition": 0
    },
    "decoration": {
        "leftHanded": false,
        "rgb": [255, 255, 255],
        "offsetX": [0, 11],
        "offsetY": [0, -65],
        "scalar": [1.0, 1.0],
        "fpsSensitivity" : 1.0,
        "fpsReturnSpeed" : 50
    },
    "osu": {
        "mouse": true,
        "toggleSmoke": false,
        "paw": [255, 255, 255],
        "pawEdge": [0, 0, 0],
        "key1": [87,83],
        "key2": [65,68],
        "smoke": [],
        "wave": []
    },
    "taiko": {
        "leftCentre": [88],
        "rightCentre": [67],
        "leftRim": [90],
        "rightRim": [86]
    },
    "catch": {
        "left": [37],
        "right": [39],
        "dash": [16]
    },
    "mania": {
        "4K": true,
        "key4K": [68, 70, 74, 75],
        "key7K": [83, 68, 70, 32, 74, 75, 76]
    },
    "custom": {
        "background": "img/osu/mousebg.png",
        "mouse": true,
        "mouseImage": "img/osu/mouse.png",
        "mouseOnTop": true,
        "offsetX": 0,
        "offsetY": 0,
        "scalar": 1.0,
        "paw": [255, 255, 255],
        "pawEdge": [0, 0, 0],
        "keyContainers": []
    },
    "gamer":{
        "cat": "img/gamer/cat.png",

        "UseCostumes": true,
        "costumeContainers": [
            {
                "costumes": [
                    {
                        "image": "img/gamer/wings.png",
                        "OnBack": true,
                        "toggleKey": [105],
                        "Name": "fairy wings"
                    },
                    {
                        "image": "img/gamer/tophat.png",
                        "OnBack": false,
                        "toggleKey": [104],
                        "Name": "tophat"
                    },
                    {
                        "image": "img/gamer/sleepy.png",
                        "OnBack": false,
                        "toggleKey": [103],
                        "Name": "sleepy hat"
                    },
                    {
                        "image": "img/gamer/unicornhorn.png",
                        "OnBack": false,
                        "toggleKey": [102],
                        "Name": "unicorn horn"
                    },
                    {
                        "image": "img/gamer/angry.png",
                        "OnBack": false,
                        "toggleKey": [99],
                        "Name": "angry face"
                    },
                    {
                        "image": "img/gamer/cry.png",
                        "OnBack": false,
                        "toggleKey": [98],
                        "Name": "cry face"
                    },
                    {
                        "image": "img/gamer/blush.png",
                        "OnBack": false,
                        "toggleKey": [97],
                        "Name": "blush face"
                    },
                    {
                        "image": "img/gamer/mustache.png",
                        "OnBack": false,
                        "toggleKey": [101],
                        "Name": "mustache"
                    }
                ]
            } 
        ],

        "desk": "img/gamer/desk.png",

        "useBackgroundImage": true,
        "backgroundImage": "img/gamer/background.png",

        "mouse": true,
        "mousePadImage": "img/gamer/mousepad.png",
        "mouseImage": "img/gamer/mouse.png",
        "mouseOnTop": true,
        "offsetX": 0,
        "offsetY": 0,
        "scalar": 3.0,
        "paw": [255, 255, 255],
        "pawEdge": [0, 0, 0],
        "UseMousePad": true,
        
        "microphone": true,
        "microphoneImage": "img/gamer/mic.png",

        "eyeBlink": true,
        "eyeClosed": "img/gamer/eyeclosed.png",
        "eyeOpen": "img/gamer/eyeopen.png",
        "blinkDuration": 6,
        "blinkMinRandomizer": 8,
        "blinkMaxRandomizer": 18,

        "mouthMicrophone": true,
        "mouthLoudVoiceOpen": "img/gamer/mouthopen.png",
        "mouthLowVoiceOpen": "img/gamer/mouthlowopen.png",
        "mouthClosed": "img/gamer/mouthclosed.png",
        
        "usePhonemes": true,
        "mouthsContainer": [
            {
                "defaultMouthClosed": "img/gamer/mouthclosed.png",
                "mouths": [
                    {
                        "LowVoiceimage": "img/gamer/mouthlowopen.png",
                        "LoudVoiceimage": "img/gamer/mouthopen.png",
                        "phoneme": "A"
                    },
                    {
                        "LowVoiceimage": "img/gamer/mouthlowopen.png",
                        "LoudVoiceimage": "img/gamer/mouthopen.png",
                        "phoneme": "E"
                    },
                    {
                        "LowVoiceimage": "img/gamer/mouthlowopen.png",
                        "LoudVoiceimage": "img/gamer/mouthopen.png",
                        "phoneme": "I"
                    },
                    {
                        "LowVoiceimage": "img/gamer/mouthlowopen.png",
                        "LoudVoiceimage": "img/gamer/mouthopen.png",
                        "phoneme": "O"
                    },
                    {
                        "LowVoiceimage": "img/gamer/mouthlowopen.png",
                        "LoudVoiceimage": "img/gamer/mouthopen.png",
                        "phoneme": "U"
                    }
                ]
            }
        ],
        
        "MicrophoneChannel": "OpenAL Soft on Microphone (Usb Audio Device)",
        "thresholdMinMicrophone": 5,
        "thresholdMicrophone": 6,
        "thresholdMicrophoneLoud": 9,

        "keyboard": true,
        "keyboardImage": "img/gamer/keyboard.png",
        "keyContainers": [
            {
                "defaultImage":"img/gamer/up.png",
                "keys":[
                    {
                        "image":"img/gamer/left.png",
                        "keyCodes":[83,32,81],
                        "joyCodes":[]
                    },
                    {
                        "image":"img/gamer/side1.png",
                        "keyCodes":[68,82],
                        "joyCodes":[]
                    },
                    {
                        "image":"img/gamer/side2.png",
                        "keyCodes":[65,16,9],
                        "joyCodes":[]
                    },
                    {
                        "image":"img/gamer/right.png",
                        "keyCodes":[87,91,69],
                        "joyCodes":[]
                    }
                ]
            }
        ]
    },
    "mousePaw": {
        "mousePawComment": "coordinates start in the top left of the window",
        "pawStartingPoint": [211, 159],
        "pawEndingPoint": [258, 228]
    }
})V0G0N";
    std::string error;
    Json::CharReaderBuilder cfg_builder;
    Json::CharReader *cfg_reader = cfg_builder.newCharReader();
    cfg_reader->parse(s, s + strlen(s), &cfg, &error);
    delete cfg_reader;
}

void error_msg(std::string error, std::string title) {
#if defined(__unix__) || defined(__unix)

    SDL_MessageBoxButtonData buttons[] = {
        { SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT, 0, "Retry" },
        { SDL_MESSAGEBOX_BUTTON_ESCAPEKEY_DEFAULT, 1, "Cancel" },
    };

    SDL_MessageBoxColorScheme colorScheme = {
        { /* .colors (.r, .g, .b) */
     /* [SDL_MESSAGEBOX_COLOR_BACKGROUND] */
        { 255, 255,255 },
        /* [SDL_MESSAGEBOX_COLOR_TEXT] */
        { 0, 0, 0 },
        /* [SDL_MESSAGEBOX_COLOR_BUTTON_BORDER] */
        { 0, 0, 0 },
        /* [SDL_MESSAGEBOX_COLOR_BUTTON_BACKGROUND] */
        { 255,255, 255 },
        /* [SDL_MESSAGEBOX_COLOR_BUTTON_SELECTED] */
        { 128, 128, 128 }
        }
    };

    SDL_MessageBoxData messagebox_data = {
    	SDL_MESSAGEBOX_ERROR,
    	NULL,
    	title.c_str(),
    	error.c_str(),
    	SDL_arraysize(buttons),
    	buttons,
    	&colorScheme
    };

    int button_id;

    SDL_ShowMessageBox(&messagebox_data, &button_id);

    if (button_id == -1 || button_id == 1) {
#else
    if (MessageBoxA(NULL, error.c_str(), title.c_str(), MB_ICONERROR | MB_RETRYCANCEL) == IDCANCEL) {
#endif
        exit(BONGO_ERROR);
    }
}

bool update(Json::Value &cfg_default, Json::Value &cfg) {
    bool is_update = true;
    for (const auto &key : cfg.getMemberNames()) {
        if (cfg_default.isMember(key)) {
            if (cfg_default[key].type() != cfg[key].type()) {
                error_msg("Value type error in config.json", "Error reading configs");
                return false;
            }
            if (cfg_default[key].isArray() && !cfg_default[key].empty()) {
                for (Json::Value &v : cfg[key]) {
                    if (v.type() != cfg_default[key][0].type()) {
                        error_msg("Value type in array error in config.json", "Error reading configs");
                        return false;
                    }
                }
            }
            if (cfg_default[key].isObject()) {
                is_update &= update(cfg_default[key], cfg[key]);
            } else {
                cfg_default[key] = cfg[key];
            }
        } else {
            cfg_default[key] = cfg[key];
        }
    }
    return is_update;
}

bool init() {
    while (true) {
        create_config();
        std::ifstream cfg_file("config.json", std::ifstream::binary);
        if (!cfg_file.good()) {
            break;
        }
        std::string cfg_string((std::istreambuf_iterator<char>(cfg_file)), std::istreambuf_iterator<char>()), error;
        Json::CharReaderBuilder cfg_builder;
        Json::CharReader *cfg_reader = cfg_builder.newCharReader();
        Json::Value cfg_read;
        if (!cfg_reader->parse(cfg_string.c_str(), cfg_string.c_str() + cfg_string.size(), &cfg_read, &error)) {
            delete cfg_reader;
            error_msg("Syntax error in config.json:\n" + error, "Error reading configs");
        } else if (update(cfg, cfg_read)) {
            delete cfg_reader;
            break;
        }
    }

    img_holder.clear();

    int mode = data::cfg["mode"].asInt();

    switch (mode) {
    case 1:
        return osu::init();
    case 2:
        return taiko::init();
    case 3:
        return ctb::init();
    case 4:
        return mania::init();
    case 5:
        return custom::init();
    case 6:
        return gamer::init();
    default:
        error_msg("Mode value is not correct", "Error reading configs");
        return false;
    }
}

sf::Texture &load_texture(std::string path) {
    if (img_holder.find(path) == img_holder.end()) {
        while (!img_holder[path].loadFromFile(path)) {
            error_msg("Cannot find file " + path, "Error importing images");
        }
    }
    return img_holder[path];
}
}; // namespace data
