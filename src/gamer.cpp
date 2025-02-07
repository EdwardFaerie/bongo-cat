#include "header.hpp"
#include <SFML/Audio.hpp>
#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <random>

bool m_istalking = false;
bool m_istalking_loud = false;
bool m_istalking_low = false;

class MicrophoneRecorder : public sf::SoundRecorder
{
    virtual bool onStart() // optional
    {
        std::cout << "Recording Microphone" << std::endl;
        return true;
    }
    virtual bool onProcessSamples(const sf::Int16 *samples, std::size_t sampleCount)
    {
        float maxAmplitude = 0.0f; // Initialize to 0.0f

        std::cout << std::to_string(sampleCount) + " Samples recorded!" << std::endl;

        // Iterate through each sample and find the maximum amplitude
        for (size_t i = 0; i < sampleCount; ++i)
        {
            float currentAmplitude = std::abs((samples[i]));
            if (currentAmplitude > maxAmplitude)
            {
                maxAmplitude = currentAmplitude;
            }
        }

        // Normalize the maximum amplitude (SFML samples are in the range of [-32768, 32767])
        float normalizedAmplitude = (maxAmplitude / 32768.0f) * 10;
        std::cout << "Max Amplitude: " << maxAmplitude << ", Normalized Amplitude: " << normalizedAmplitude << std::endl;
        Json::Value custom = data::cfg["gamer"];

        if (normalizedAmplitude < custom["thresholdMinMicrophone"].asDouble())
        {
            m_istalking = false;
            m_istalking_loud = false;
            m_istalking_low = false;
        }
        else
        {
            m_istalking = true;
            if (normalizedAmplitude > custom["thresholdMicrophoneLoud"].asDouble())
            {
                m_istalking_loud = true;
            }
            else
            {
                m_istalking_loud = false;

                if (normalizedAmplitude > custom["thresholdMicrophone"].asDouble())
                {
                    m_istalking_low = true;
                }
                else
                {
                    m_istalking_low = false;
                }
            }
        }
        return true;
    }

    virtual void onStop() // optional
    {
        std::cout << "Stopped Recording Microphone" << std::endl;
    }
};

namespace gamer
{
    bool recorderBool = false;

    struct key
    {
        Json::Value key_value;
        Json::Value joy_value;
        sf::Sprite sprite;
        bool status;
        double timer;

        key(Json::Value _key_value)
        {
            sprite = sf::Sprite();
            if (_key_value.isMember("keyCodes") && _key_value["keyCodes"].isArray())
            {
                key_value = _key_value["keyCodes"];
            }
            else
            {
                data::error_msg("Custom keyCodes values is not set correctly", "Error reading configs");
                throw;
            }
            if (_key_value.isMember("image") && _key_value["image"].isString())
            {
                sprite.setTexture(data::load_texture(_key_value["image"].asString()));
            }
            else
            {
                data::error_msg("Custom image path is not set correctly", "Error reading configs");
                throw;
            }
            if (_key_value.isMember("joyCodes"))
            {
                if (!_key_value["joyCodes"].isArray())
                {
                    data::error_msg("Custom joyCodes values is not set correctly", "Error reading configs");
                    throw;
                }
                joy_value = _key_value["joyCodes"];
            }
            status = false;
            timer = -1;
        }

        bool is_pressed()
        {
            for (Json::Value &v : key_value)
            {
                if (input::is_pressed(v.asInt()))
                {
                    return true;
                }
            }

            if (input::is_joystick_connected())
            {
                for (Json::Value &v : joy_value)
                {
                    if (input::is_joystick_pressed(v.asInt()))
                    {
                        return true;
                    }
                }
            }

            return false;
        }

        void draw()
        {
            window.draw(sprite);
            timer = clock();
        }
    };

    bool bool1, bool2, bool3;
    

    struct costumes_key
    {
        Json::Value key_value;
        bool onBackVV;
        sf::Sprite sprite;
        bool activated, status;
        std::string Name;

        costumes_key(Json::Value costumes_container_value)
        {
            sprite = sf::Sprite();
            if (costumes_container_value.isMember("toggleKey") && costumes_container_value["toggleKey"].isArray())
            {
                key_value = costumes_container_value["toggleKey"];
            }
            else
            {
                data::error_msg("Custom toggleKey values is not set correctly", "Error reading configs");
                throw;
            }
            if (costumes_container_value.isMember("image") && costumes_container_value["image"].isString())
            {
                sprite.setTexture(data::load_texture(costumes_container_value["image"].asString()));
                Name = costumes_container_value["Name"].asString();
            }
            else
            {
                data::error_msg("Custom image path is not set correctly", "Error reading configs");
                throw;
            }
            if (costumes_container_value.isMember("OnBack") && costumes_container_value["OnBack"].isBool())
            {
                onBackVV = costumes_container_value["OnBack"].asBool();
            }
            else
            {
                data::error_msg("Custom OnBack is not set correctly", "Error reading configs");
                throw;
            }
            activated = false; // Initialize activated state
            status = false;
        }

        bool is_pressed()
        {
            bool currentlyPressed = false;
            for (Json::Value &v : key_value)
            {
                if (input::is_pressed(v.asInt()))
                {
                    currentlyPressed = true;
                    break;
                }
            }
            return currentlyPressed;
        }

        void draw()
        {
            window.draw(sprite);
        }
    };

    struct costumes_container
    {
        std::vector<costumes_key> costumes;
        sf::Sprite default_sprite;
        int key_state;

        costumes_container(Json::Value key_container_value)
        {
            key_state = -1;
            if (key_container_value.isObject())
            {
                for (Json::Value &child_key : key_container_value["costumes"])
                {
                    costumes.push_back(costumes_key(child_key));
                }
            }
            else
            {
                data::error_msg("costumes container must be an object", "Error reading configs");
                throw;
            }
        }

        void draw(bool Value)
        {
            for (int i = 0; i < costumes.size(); i++)
            {
                costumes_key &current_key = costumes[i];

                if (current_key.is_pressed() && !current_key.status) // Check if pressed and was not pressed before
                {
                    current_key.activated = !current_key.activated; // Toggle activated state
                }
                current_key.status = current_key.is_pressed(); // Update the status

                if (current_key.activated)
                {
                    if ((current_key.onBackVV == Value) == true)
                    {
                        current_key.draw();
                    }
                }
            }
        }
    };

    struct key_container
    {
        std::vector<key> keys;
        sf::Sprite default_sprite;
        int key_state;

        key_container(Json::Value key_container_value)
        {
            key_state = -1;
            if (key_container_value.isObject())
            {
                if (!key_container_value.isMember("defaultImage") || !key_container_value["defaultImage"].isString() || !key_container_value.isMember("keys") || !key_container_value["keys"].isArray())
                {
                    data::error_msg("Key container's object error", "Error reading configs");
                    throw;
                }
                else
                {
                    default_sprite = sf::Sprite();
                    default_sprite.setTexture(data::load_texture(key_container_value["defaultImage"].asString()));
                    for (Json::Value &child_key : key_container_value["keys"])
                    {
                        keys.push_back(key(child_key));
                    }
                }
            }
            else
            {
                data::error_msg("Key container must be an object", "Error reading configs");
                throw;
            }
        }

        void draw()
        {
            bool is_any_key_pressed = false;
            for (int i = 0; i < keys.size(); i++)
            {
                key &current_key = keys[i];
                if (current_key.is_pressed())
                {
                    is_any_key_pressed = true;
                    if (!current_key.status)
                    {
                        key_state = i;
                        current_key.status = true;
                    }
                }
                else
                {
                    current_key.status = false;
                }
            }
            if (!is_any_key_pressed)
            {
                key_state = -1;
                window.draw(default_sprite);
            }
            if (key_state > -1)
            {
                key &on_key = keys[key_state];
                double last_press = -1;
                for (int i = 0; i < (int)keys.size(); i++)
                {
                    if (i != key_state)
                    {
                        last_press = std::max(last_press, keys[i].timer);
                    }
                }
                if ((clock() - last_press) / CLOCKS_PER_SEC > BONGO_KEYPRESS_THRESHOLD)
                {
                    on_key.draw();
                }
                else
                {
                    window.draw(default_sprite);
                }
            }
        }
    };

    std::vector<key_container> key_containers;
    std::vector<costumes_container> costumes_containers;
    sf::Sprite keyboard, mic, cat, desk, mouse, mousePad, mouthOpen, mouthlowOpen, mouthClosed, noMouseHand, noKeyboardHand;
    sf::Sprite catCostume1, catCostume2, catCostume3, background, openEyeSprite, ClosedEyeSprite;

    MicrophoneRecorder recorder;
    std::string microphoneChannelName; // Store the microphone channel name dynamically
    bool microphoneFound = false;

    void exiting()
    {
        recorder.stop();
        std::cout << "Exiting software";
    };

    bool use_blink = false;
    bool isBlinking = false;
    sf::Clock blinkClock;
    float blinkDuration; // Duration of the blink in seconds
    float blinkInterval; // Random interval for the next blink
    float blinkx;
    float blinky;
    std::default_random_engine generator;
    std::uniform_real_distribution<float> distribution(blinkx, blinky); // Random interval between seconds

    void resetBlinkInterval()
    {
        blinkInterval = distribution(generator);
        blinkClock.restart(); // Reset the clock
    }

    bool is_mic, is_mouth, is_Keyboard, is_mouse, is_mouse_on_top;
    int offset_x, offset_y, scale, THRESHOLD;
    int paw_r, paw_g, paw_b, paw_a;
    int paw_edge_r, paw_edge_g, paw_edge_b, paw_edge_a;
    bool is_mousePad, use_background, UseCostumes;

    bool SetMicrophone()
    {
        std::vector<std::string> devices = sf::SoundRecorder::getAvailableDevices();

        // Check if the configured microphone channel exists in the available devices
        for (const std::string &device : devices)
        {
            if (device == microphoneChannelName)
            {
                std::cout << "Microphone Found: " << microphoneChannelName << std::endl;
                // If found, initialize the recorder with the selected device
                recorder.setDevice(device);
                microphoneFound = true;
            }
        }

        if (!microphoneFound)
        {
            std::vector<std::string> devices = sf::SoundRecorder::getAvailableDevices();
            std::string allDevices;
            std::ostringstream oss;

            for (size_t i = 0; i < devices.size(); ++i) {
                oss << devices[i];
                if (i < devices.size() - 1) {
                    oss << ", "; // Add a comma separator between device names
                }
            }
            allDevices = oss.str();

            // Open file for writing using std::ofstream (no need for freopen or std::cout)
            std::ofstream outFile("MicrophonesList.txt");
            
            if (!outFile) {
                // If the file could not be opened, print an error message
                std::cerr << "Error opening file for writing." << std::endl;
            }

            // Write to the file
            outFile << "Devices: " << allDevices << std::endl;
            
            // Close the file when you're done
            outFile.close();

            // Optional: Log an error message using std::cerr if needed
            std::cerr << "Failed to find selected MicrophoneChannel, look for your device in MicrophonesList.txt" << std::endl;
            return false;
        }
        else
        {
            return true;
        }
    }

    bool init()
    {
        // getting configs
        try
        {
            freopen("ErrorLog.txt", "w", stderr);
            freopen("OutputLog.txt", "w", stdout);

            Json::Value custom = data::cfg["gamer"];

            use_background = custom["useBackgroundImage"].asBool();
            if (use_background)
            {
                background.setTexture(data::load_texture(custom["backgroundImage"].asString()));
            }

            // Base part for Costumes toggles
            UseCostumes = custom["UseCostumes"].asBool();
            if (UseCostumes)
            {
                costumes_containers.clear();
                for (Json::Value &costume_keys : custom["costumeContainers"])
                {
                    costumes_containers.push_back(costumes_container(costume_keys));
                }
            }

            if (custom.isMember("MicrophoneChannel") && custom["MicrophoneChannel"].isString())
            {
                microphoneChannelName = custom["MicrophoneChannel"].asString();
                std::cout << "Configured Microphone Channel: " << microphoneChannelName << std::endl;
            }
            else
            {
                data::error_msg("Custom MicrophoneChannel not found", "Error reading config");
                return false;
            }

            is_mousePad = custom["UseMousePad"].asBool();

            bool microphoneFunc = SetMicrophone();

            if (microphoneFunc == false)
            {
                data::error_msg("Microphone error. Check 'MicrophonesList.txt' in bongo.exe folder", "Error with microphone");
                return false;
            }

            is_Keyboard = custom["keyboard"].asBool();
            if (is_Keyboard)
            {
                key_containers.clear();
                for (Json::Value &current_key_container : custom["keyContainers"])
                {
                    key_containers.push_back(key_container(current_key_container));
                }
            }
            keyboard.setTexture(data::load_texture(custom["keyboardImage"].asString()));

            if (!custom.isMember("cat") || !custom["cat"].isString())
            {
                data::error_msg("Custom cat not found", "Error reading config");
                return false;
            }
            cat.setTexture(data::load_texture(custom["cat"].asString()));

            if (!custom.isMember("desk") || !custom["desk"].isString())
            {
                data::error_msg("Custom desk not found", "Error reading config");
                return false;
            }
            desk.setTexture(data::load_texture(custom["desk"].asString()));

            is_mouth = custom["mouthMicrophone"].asBool();
            if (!custom.isMember("mouthLoudVoiceOpen") || !custom["mouthLoudVoiceOpen"].isString())
            {
                data::error_msg("Custom mouthLoudVoiceOpen not found", "Error reading config");
                return false;
            }
            if (!custom.isMember("mouthLowVoiceOpen") || !custom["mouthLowVoiceOpen"].isString())
            {
                data::error_msg("Custom mouthLowVoiceOpen not found", "Error reading config");
                return false;
            }
            if (!custom.isMember("mouthClosed") || !custom["mouthClosed"].isString())
            {
                data::error_msg("Custom mouthClosed not found", "Error reading config");
                return false;
            }
            mouthOpen.setTexture(data::load_texture(custom["mouthLoudVoiceOpen"].asString()));
            mouthClosed.setTexture(data::load_texture(custom["mouthClosed"].asString()));
            mouthlowOpen.setTexture(data::load_texture(custom["mouthLowVoiceOpen"].asString()));

            is_mic = custom["microphone"].asBool();
            if (!custom.isMember("microphoneImage") || !custom["microphoneImage"].isString())
            {
                data::error_msg("Custom microphoneImage not found", "Error reading config");
                return false;
            }
            mic.setTexture(data::load_texture(custom["microphoneImage"].asString()));

            noMouseHand.setTexture(data::load_texture("img/gamer/noMouseHand.png"));
            noKeyboardHand.setTexture(data::load_texture("img/gamer/noKeyboardHand.png"));

            THRESHOLD = custom["thresholdMicrophone"].asDouble();

            is_mouse = custom["mouse"].asBool();
            if (is_mouse)
            {
                is_mouse_on_top = custom["mouseOnTop"].asBool();

                offset_x = custom["offsetX"].asInt();
                offset_y = custom["offsetY"].asInt();
                scale = custom["scalar"].asDouble();

                paw_r = custom["paw"][0].asInt();
                paw_g = custom["paw"][1].asInt();
                paw_b = custom["paw"][2].asInt();
                paw_a = custom["paw"].size() == 3 ? 255 : custom["paw"][3].asInt();

                paw_edge_r = custom["pawEdge"][0].asInt();
                paw_edge_g = custom["pawEdge"][1].asInt();
                paw_edge_b = custom["pawEdge"][2].asInt();
                paw_edge_a = custom["pawEdge"].size() == 3 ? 255 : custom["pawEdge"][3].asInt();

                if (!custom.isMember("mouseImage") || !custom["mouseImage"].isString())
                {
                    data::error_msg("Mouse image not found", "Error reading config");
                    return false;
                }
                mouse.setTexture(data::load_texture(custom["mouseImage"].asString()));

                if (!custom.isMember("mousePadImage") || !custom["mousePadImage"].isString())
                {
                    data::error_msg("MousePad image not found", "Error reading config");
                    return false;
                }
                mousePad.setTexture(data::load_texture(custom["mousePadImage"].asString()));
            }

            if (!recorder.isAvailable())
            {
                std::cerr << "Não é possivel usar o microfone selecionado!!" << std::endl;
                return false;
            }

            if (!recorderBool && is_mouth || !recorder.start(44100))
            {
                std::cout << "Iniciando o gravador!" << std::endl;
                recorder.start(44100);
                recorderBool = true;
            }

            use_blink = custom["eyeBlink"].asBool();

            if (use_blink)
            {
                if (!custom.isMember("eyeClosed") || !custom["eyeClosed"].isString())
                {
                    data::error_msg("eyeclosed image not found", "Error reading config");
                    return false;
                }
                blinkx = custom["blinkMinRandomizer"].asFloat();
                blinky = custom["blinkMaxRandomizer"].asFloat();
                blinkDuration = custom["blinkDuration"].asFloat();
                ClosedEyeSprite.setTexture(data::load_texture(custom["eyeClosed"].asString()));
                resetBlinkInterval(); // Set the initial blink interval
            }

            if (!custom.isMember("eyeOpen") || !custom["eyeOpen"].isString())
            {
                data::error_msg("eyeOpen image not found", "Error reading config");
                return false;
            }
            openEyeSprite.setTexture(data::load_texture(custom["eyeOpen"].asString()));

            std::atexit(exiting);
        }
        catch (...)
        {
            return false;
        }
        return true;
    }

    struct blink
    {
        void Update()
        {
            // Check if we are currently blinking
            if (isBlinking)
            {
                // If the blink duration has elapsed, stop blinking
                if (blinkClock.getElapsedTime().asSeconds() > blinkDuration)
                {
                    isBlinking = false;   // Stop blinking
                    resetBlinkInterval(); // Set the next blink interval
                }
            }
            else
            {
                // If not blinking, check if it's time to start a new blink
                if (blinkClock.getElapsedTime().asSeconds() > blinkInterval)
                {
                    isBlinking = true;    // Start blinking
                    blinkClock.restart(); // Restart the clock for the blink duration
                }
            }
        }
    };

    blink blinkManager;

    void draw()
    {

        if (use_background)
        {
            window.draw(background);
        }

        if (UseCostumes)
        {
            for (costumes_container &current : costumes_containers)
            {
                current.draw(true);
            }
        }

        window.draw(cat);

        if (use_blink)
        {
            blinkManager.Update();
            // Draw the eye based on the blinking state
            if (isBlinking)
            {
                window.draw(openEyeSprite);
            }
            else
            {
                window.draw(ClosedEyeSprite);
            }
        }

        if (!use_blink)
        {
            window.draw(openEyeSprite);
        }

        window.draw(desk);

        if (is_mousePad && is_mouse)
        {
            window.draw(mousePad);
        }

        if (is_mouth)
        {
            
            if (m_istalking && !m_istalking_loud && !m_istalking_low)
            {
                window.draw(mouthlowOpen); // Display Low mouth open when talking
            }
            else if (m_istalking && m_istalking_loud)
            {
                window.draw(mouthOpen); // Display Loud mouth open when talking
            }
            else if (m_istalking && m_istalking_low)
            {
                window.draw(mouthlowOpen); // Display low mouth open when talking
            }
            else if (m_istalking && m_istalking_loud && m_istalking_low)
            {
                window.draw(mouthOpen); // Display Loud mouth open when talking
            }
            else if (!m_istalking)
            {
                window.draw(mouthClosed); // Display mouth closed when quiet
            }
        }
        else
        {
            window.draw(mouthClosed); // Display mouth closed when quiet
        }

        if (is_Keyboard)
        {
            window.draw(keyboard);
            for (key_container &current : key_containers)
            {
                current.draw();
            }
        }
        else
        {
            window.draw(noKeyboardHand);
        }

        if (is_mouse)
        {
            // initializing pss and pss2 (kuvster's magic)
            Json::Value paw_draw_info = data::cfg["mousePaw"];
            int x_paw_start = paw_draw_info["pawStartingPoint"][0].asInt();
            int y_paw_start = paw_draw_info["pawStartingPoint"][1].asInt();
            auto [x, y] = input::get_xy();
            int oof = 6;
            std::vector<double> pss = {(float)x_paw_start, (float)y_paw_start};
            double dist = hypot(x_paw_start - x, y_paw_start - y);
            double centreleft0 = x_paw_start - 0.7237 * dist / 2;
            double centreleft1 = y_paw_start + 0.69 * dist / 2;
            for (int i = 1; i < oof; i++)
            {
                std::vector<double> bez = {(float)x_paw_start, (float)y_paw_start, centreleft0, centreleft1, x, y};
                auto [p0, p1] = input::bezier(1.0 * i / oof, bez, 6);
                pss.push_back(p0);
                pss.push_back(p1);
            }
            pss.push_back(x);
            pss.push_back(y);
            double a = y - centreleft1;
            double b = centreleft0 - x;
            double le = hypot(a, b);
            a = x + a / le * 60;
            b = y + b / le * 60;
            int x_paw_end = paw_draw_info["pawEndingPoint"][0].asInt();
            int y_paw_end = paw_draw_info["pawEndingPoint"][1].asInt();
            dist = hypot(x_paw_end - a, y_paw_end - b);
            double centreright0 = x_paw_end - 0.6 * dist / 2;
            double centreright1 = y_paw_end + 0.8 * dist / 2;
            int push = 20;
            double s = x - centreleft0;
            double t = y - centreleft1;
            le = hypot(s, t);
            s *= push / le;
            t *= push / le;
            double s2 = a - centreright0;
            double t2 = b - centreright1;
            le = hypot(s2, t2);
            s2 *= push / le;
            t2 *= push / le;
            for (int i = 1; i < oof; i++)
            {
                std::vector<double> bez = {x, y, x + s, y + t, a + s2, b + t2, a, b};
                auto [p0, p1] = input::bezier(1.0 * i / oof, bez, 8);
                pss.push_back(p0);
                pss.push_back(p1);
            }
            pss.push_back(a);
            pss.push_back(b);
            for (int i = oof - 1; i > 0; i--)
            {
                std::vector<double> bez = {1.0 * x_paw_end, 1.0 * y_paw_end, centreright0, centreright1, a, b};
                auto [p0, p1] = input::bezier(1.0 * i / oof, bez, 6);
                pss.push_back(p0);
                pss.push_back(p1);
            }
            pss.push_back(x_paw_end);
            pss.push_back(y_paw_end);
            double mpos0 = (a + x) / 2 - 52 - 15;
            double mpos1 = (b + y) / 2 - 34 + 5;
            double dx = -38;
            double dy = -50;

            const int iter = 25;

            std::vector<double> pss2 = {pss[0] + dx, pss[1] + dy};
            for (int i = 1; i < iter; i++)
            {
                auto [p0, p1] = input::bezier(1.0 * i / iter, pss, 38);
                pss2.push_back(p0 + dx);
                pss2.push_back(p1 + dy);
            }
            pss2.push_back(pss[36] + dx);
            pss2.push_back(pss[37] + dy);

            mouse.setPosition(mpos0 + dx + offset_x, mpos1 + dy + offset_y);

            if (is_mouse_on_top)
            {
                window.draw(mouse);
            }

            // drawing arms
            sf::VertexArray fill(sf::TriangleStrip, 26);
            for (int i = 0; i < 26; i += 2)
            {
                fill[i].position = sf::Vector2f(pss2[i], pss2[i + 1]);
                fill[i + 1].position = sf::Vector2f(pss2[52 - i - 2], pss2[52 - i - 1]);
                fill[i].color = sf::Color(paw_r, paw_g, paw_b, paw_a);
                fill[i + 1].color = sf::Color(paw_r, paw_g, paw_b, paw_a);
            }
            window.draw(fill);

            // drawing first arm arc
            int shad = paw_edge_a / 3;
            sf::VertexArray edge(sf::TriangleStrip, 52);
            double width = 7;
            sf::CircleShape circ(width / 2);
            circ.setFillColor(sf::Color(paw_edge_r, paw_edge_g, paw_edge_b, shad));
            circ.setPosition(pss2[0] - width / 2, pss2[1] - width / 2);
            window.draw(circ);
            for (int i = 0; i < 50; i += 2)
            {
                double vec0 = pss2[i] - pss2[i + 2];
                double vec1 = pss2[i + 1] - pss2[i + 3];
                double dist = hypot(vec0, vec1);
                edge[i].position = sf::Vector2f(pss2[i] + vec1 / dist * width / 2, pss2[i + 1] - vec0 / dist * width / 2);
                edge[i + 1].position = sf::Vector2f(pss2[i] - vec1 / dist * width / 2, pss2[i + 1] + vec0 / dist * width / 2);
                edge[i].color = sf::Color(paw_edge_r, paw_edge_g, paw_edge_b, shad);
                edge[i + 1].color = sf::Color(paw_edge_r, paw_edge_g, paw_edge_b, shad);
                width -= 0.08;
            }
            double vec0 = pss2[50] - pss2[48];
            double vec1 = pss2[51] - pss2[49];
            dist = hypot(vec0, vec1);
            edge[51].position = sf::Vector2f(pss2[50] + vec1 / dist * width / 2, pss2[51] - vec0 / dist * width / 2);
            edge[50].position = sf::Vector2f(pss2[50] - vec1 / dist * width / 2, pss2[51] + vec0 / dist * width / 2);
            edge[50].color = sf::Color(paw_edge_r, paw_edge_g, paw_edge_b, shad);
            edge[51].color = sf::Color(paw_edge_r, paw_edge_g, paw_edge_b, shad);
            window.draw(edge);
            circ.setRadius(width / 2);
            circ.setPosition(pss2[50] - width / 2, pss2[51] - width / 2);
            window.draw(circ);

            // drawing second arm arc
            sf::VertexArray edge2(sf::TriangleStrip, 52);
            width = 6;
            sf::CircleShape circ2(width / 2);
            circ2.setFillColor(sf::Color(paw_edge_r, paw_edge_g, paw_edge_b, paw_edge_a));
            circ2.setPosition(pss2[0] - width / 2, pss2[1] - width / 2);
            window.draw(circ2);
            for (int i = 0; i < 50; i += 2)
            {
                vec0 = pss2[i] - pss2[i + 2];
                vec1 = pss2[i + 1] - pss2[i + 3];
                double dist = hypot(vec0, vec1);
                edge2[i].position = sf::Vector2f(pss2[i] + vec1 / dist * width / 2, pss2[i + 1] - vec0 / dist * width / 2);
                edge2[i + 1].position = sf::Vector2f(pss2[i] - vec1 / dist * width / 2, pss2[i + 1] + vec0 / dist * width / 2);
                edge2[i].color = sf::Color(paw_edge_r, paw_edge_g, paw_edge_b, paw_edge_a);
                edge2[i + 1].color = sf::Color(paw_edge_r, paw_edge_g, paw_edge_b, paw_edge_a);
                width -= 0.08;
            }
            vec0 = pss2[50] - pss2[48];
            vec1 = pss2[51] - pss2[49];
            dist = hypot(vec0, vec1);
            edge2[51].position = sf::Vector2f(pss2[50] + vec1 / dist * width / 2, pss2[51] - vec0 / dist * width / 2);
            edge2[50].position = sf::Vector2f(pss2[50] - vec1 / dist * width / 2, pss2[51] + vec0 / dist * width / 2);
            edge2[50].color = sf::Color(paw_edge_r, paw_edge_g, paw_edge_b, paw_edge_a);
            edge2[51].color = sf::Color(paw_edge_r, paw_edge_g, paw_edge_b, paw_edge_a);
            window.draw(edge2);
            circ2.setRadius(width / 2);
            circ2.setPosition(pss2[50] - width / 2, pss2[51] - width / 2);
            window.draw(circ2);
        }

        // drawing mouse at the bottom
        if (is_mouse && !is_mouse_on_top)
        {
            window.draw(mouse);
        }

        if (!is_mouse)
        {
            window.draw(noMouseHand);
        }

        if (is_mic)
        {
            window.draw(mic);
        }

        if (UseCostumes)
        {
            for (costumes_container &current : costumes_containers)
            {
                current.draw(false);
            }
        }
    }

}; // namespace custom