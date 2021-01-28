#include <iostream>
#include <cmath>
#include <vector>

//
//

int ticks = 0;
constexpr unsigned int nodes = 9;
constexpr unsigned int subticks = 12;
constexpr unsigned int tick_update_parameter = 1000;

//
//

struct node {
    node() = default;
    int id;
    float intensity = 1.0f;
    bool animation_on = false;
    //int ticks_flickering = 0;
    //int ticks_to_flicker = 0;
    int ticks_since_last_change = 0.0f;
    float animation[100]; //intensities for the next 100 ticks
    std::vector<bool>on_subtick;
};

std::vector<node>lights;

//
//

bool node_needs_update(int node);
void setup();
void update();
void fade(int node);
//void flickering(int node);
void fill_gap_animation(int node, float start_intensity, float end_intensity);
void generate_subtick_pattern(int node, int animation_tick);
bool node_on_tick(int node, int subtick);
void display();

//
//

int main()
{
    setup();
    
    //while (true) { update(); }
    
    fill_gap_animation(0, 0.5f, 1.0f);
    generate_subtick_pattern(0, 0);
    generate_subtick_pattern(0, 1);
    display();

    return 0;
}

bool node_needs_update(int node) {
    if (lights[node].ticks_since_last_change > (rand() % tick_update_parameter)) {
        //std::cout << "node: " << node << " needs to be updated" << std::endl;
        return true;
    }
    return false;
}

void setup() {
    for (size_t i = 0; i < nodes; i++) {
        lights.push_back(node());
    }
    int id = 0;
    for (auto& light : lights) {
        light.id = id;
        id++;
        light.ticks_since_last_change = (std::rand() % 1000);
        for (int i = 0; i < subticks; i++) {
            light.on_subtick.push_back(false);
        }
    }
}

void update() {
    for (auto& light: lights) {
        if (node_needs_update(light.id)) {
            if (light.ticks_since_last_change % 2) {
               //->flickering
                //flickering(light.id);
            }
            else {
               //->intensity
                fade(light.id);
                //std::cout << "intensity changed to: " << light.intensity << std::endl;
            }
        }
        else {
            light.ticks_since_last_change++;
        }
    }

    ticks++;
}

void fade(int node) {
    float factor = 1; //algo für factor nötig
    float target_intensity = lights[node].intensity * factor;
    fill_gap_animation(node, lights[node].intensity, target_intensity);
    lights[node].ticks_since_last_change = 0;
    lights[node].animation_on = true;
}

void fill_gap_animation(int node, float start_intensity, float end_intensity) {
    float f = (end_intensity - start_intensity) / 100;
    for (int i = 0; i < 100; i++) {
        lights[node].animation[i] = start_intensity + ((1 + i) * f);
        //std::cout << lights[node].animation[i] << std::endl;
    }
}

void generate_subtick_pattern(int node, int animation_tick) {
    float temp = lights[node].animation[animation_tick];
    int n_on = 0;
    for (int i = 0; i < subticks; i++) {
        if (temp <= (i * (1 / subticks))) {
            n_on = i;
        }
        //clear old pattern
        lights[node].on_subtick[i] = 0;
    }
    //write new pattern, better ditribution needed
    for (int i = 0; i < n_on; i++) {
        lights[node].on_subtick[i] = true;
    }

}

bool node_on_tick(int node, int subtick) {
    if (lights[node].on_subtick[subtick]) {
        return true;
    }
    return false;
}

void display() {
    for (int an_pattern= 0; an_pattern < 10; an_pattern++) {
        for (int i = 0; i < nodes; i++) {
            generate_subtick_pattern(i, an_pattern);
        }
        for (int subtick = 0; subtick < 12; subtick++) {
            std::cout << "subtick: " << subtick << std::endl;
            for (int i = 0; i < nodes; i++) {
                if (node_on_tick(i, subtick)) {
                    std::cout << "node " << i << ":  On" << std::endl;
                    //pin->on/off
                }
                else {
                    std::cout << "node " << i << ":  Off" << std::endl;
                }
            }
        }
        std::cout << std::endl;
    }
}
