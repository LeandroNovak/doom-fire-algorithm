#include <SFML/Graphics.hpp>
#include <cmath>

#define WIDTH 240
#define HEIGHT 160

const sf::Uint8 fireColorsPalette[] {
    7, 7, 7, 255,
    31, 7, 7, 255,
    47, 15, 7, 255,
    71, 15, 7, 255,
    87, 23, 7, 255,
    103, 31, 7, 255,
    119, 31, 7, 255,
    143, 39, 7, 255,
    159, 47, 7, 255,
    175, 63, 7, 255,
    191, 71, 7, 255,
    199, 71, 7, 255,
    223, 79, 7, 255,
    223, 87, 7, 255,
    223, 87, 7, 255,
    215, 95, 7, 255,
    215, 95, 7, 255,
    215, 103, 15, 255,
    207, 111, 15, 255,
    207, 119, 15, 255,
    207, 127, 15, 255,
    207, 135, 23, 255,
    199, 135, 23, 255,
    199, 143, 23, 255,
    199, 151, 31, 255,
    191, 159, 31, 255,
    191, 159, 31, 255,
    191, 167, 39, 255,
    191, 167, 39, 255,
    191, 175, 47, 255,
    183, 175, 47, 255,
    183, 183, 47, 255,
    183, 183, 55, 255,
    207, 207, 111, 255,
    223, 223, 159, 255,
    239, 239, 199, 255,
    255, 255, 255, 255
};

sf::RenderWindow window;
sf::Texture texture;
sf::Sprite sprite;
sf::Uint8 pixels[WIDTH * HEIGHT * 4];
int firePixelsArray[WIDTH * HEIGHT]; 
bool paused;

void initializeFireDataStructure() {
    for (int i = 0; i < WIDTH * HEIGHT; i++)
        firePixelsArray[i] = 0;
}

void createFireSource() {
    for (int i = 0; i < WIDTH; i++)
        firePixelsArray[WIDTH * (HEIGHT - 1) + i] = 36;
}

void draw() {
    for (int i = 0; i < WIDTH; i++) {
        for (int j = 0; j < HEIGHT; j++) {
            pixels[(i + (j * WIDTH)) * 4 + 0] = 
                fireColorsPalette[firePixelsArray[i + (j * WIDTH)] * 4 + 0];
            pixels[(i + (j * WIDTH)) * 4 + 1] = 
                fireColorsPalette[firePixelsArray[i + (j * WIDTH)] * 4 + 1];
            pixels[(i + (j * WIDTH)) * 4 + 2] = 
                fireColorsPalette[firePixelsArray[i + (j * WIDTH)] * 4 + 2];
            pixels[(i + (j * WIDTH)) * 4 + 3] = 
                fireColorsPalette[firePixelsArray[i + (j * WIDTH)] * 4 + 3];
        }
    }

    texture.update(pixels);

    window.clear(sf::Color::Black);
    window.draw(sprite);
    window.display();
}

void updateFireIntensityPerPixel(int currentPixelIndex) {
    int belowPixelIndex = currentPixelIndex + WIDTH;
    
    if (belowPixelIndex >= WIDTH * HEIGHT)
        return;

    int decay = floor(rand() / double(RAND_MAX) * 3);
    int belowPixelFireIntensity = firePixelsArray[belowPixelIndex];
    int newFireIntensity = belowPixelFireIntensity - decay >= 0 ?
        belowPixelFireIntensity - decay : 0;
    firePixelsArray[currentPixelIndex - decay] = newFireIntensity;
}

void calculateFirePropagation() {
    for (int i = 0; i < WIDTH; i++)
        for (int j = 0; j < HEIGHT; j++)
            updateFireIntensityPerPixel(i + (WIDTH * j));

    draw();
}

int main() {
    window.create(sf::VideoMode(WIDTH, HEIGHT), "Doom Fire");
    window.setFramerateLimit(30);

    texture.create(WIDTH, HEIGHT);
    sprite.setTexture(texture);
    paused = false;

    initializeFireDataStructure();
    createFireSource();
    
    while(window.isOpen()) {
        sf::Event event;
        while(window.pollEvent(event)) {
            switch (event.type) {
                case sf::Event::Closed:
                    window.close();
                    break;
                case sf::Event::KeyPressed:
                    if (event.key.code == sf::Keyboard::Space)
                        paused = !paused;
                    break;
            }
        }

        if (!paused)
            calculateFirePropagation();
    }

    return 0;
}
