//
//  main.cpp
//  train_P
//
//  Created by Golam Rabbani
//

#include <GLUT/glut.h>
#include <iostream>
#include <string>
#include <cmath>
using namespace std;


bool trainMoving = true; // Train starts moving by default

// Global Variables
#define PI 3.14159265

float smokeY = 0.0f;
bool dayNight = true;       // Toggle for day or night mode
float trainPosition = 1.2f; // Train's horizontal position
float trainSpeed = 0.01f;   // Train's speed

// Set color based on the mode (day or night)
void setColor(const std::string &col) {
    if (dayNight) { // Day Mode
        if (col == "trainBody") glColor3f(0.4f, 0.47f, 0.52f);
        else if (col == "trainGlass") glColor3f(0.57f, 0.66f, 0.69f);
        else if (col == "buildingGlass") glColor3f(0.75f, 0.85f, 0.9f);
        else if (col == "trainStripe") glColor3f(0.58f, 0.65f, 0.69f);
        else if (col == "building") glColor3f(0.7f, 0.7f, 0.75f);
        else if (col == "bridge") glColor3f(0.6f, 0.6f, 0.65f);
        else if (col == "trainFront") glColor3f(0.4f, 0.47f, 0.52f);
        else if (col == "trainStripe") glColor3f(1.0f, 0.0f, 0.0f);
        else if (col == "laneMarking") glColor3f(1.0f, 1.0f, 1.0f);
        else if (col == "light") glColor3f(1.0f, 0.94f, 0.96f);
        else if (col == "road") glColor3f(0.2f, 0.2f, 0.2f); // Dark gray for the road
        else if (col == "wheel") glColor3f(0.2f, 0.2f, 0.2f); // Dark gray for wheels
        else glColor3f(0.0f, 0.0f, 0.0f); // Default black
    } else { // Night Mode
        if (col == "trainBody") glColor3f(0.21f, 0.33f, 0.45f);
        else if (col == "trainGlass") glColor3f(1.0f, 1.0f, 1.0f);
        else if (col == "buildingGlass") glColor3f(0.9647f, 0.9412f, 0.5922f);
        else if (col == "trainStripe") glColor3f(0.26f, 0.46f, 0.55f);
        else if (col == "building") glColor3f(0.4f, 0.4f, 0.5f);
        else if (col == "bridge") glColor3f(0.3f, 0.3f, 0.4f);
        else if (col == "light") glColor3f(1.0f, 0.94f, 0.96f);
        else if (col == "trainFront") glColor3f(0.4f, 0.47f, 0.52f);
        else if (col == "laneMarking") glColor3f(0.8f, 0.8f, 0.8f);
        else if (col == "road") glColor3f(0.2f, 0.2f, 0.2f); // Even darker gray for the road
        else if (col == "wheel") glColor3f(0.1f, 0.1f, 0.1f); // Darker gray for wheels
        else glColor3f(0.0f, 0.0f, 0.0f); // Default black
    }
}

// Draw gradient sky
void drawGradientSky() {
    glBegin(GL_QUADS);
    if (dayNight) { // Day Mode
        glColor3f(0.53f, 0.81f, 0.98f); // Light blue (top)
        glVertex2f(-1.0f, 1.0f);
        glVertex2f(1.0f, 1.0f);
        glColor3f(0.87f, 0.92f, 1.0f); // Very light blue (bottom)
        glVertex2f(1.0f, -1.0f);
        glVertex2f(-1.0f, -1.0f);
    } else { // Night Mode
        glColor3f(0.0f, 0.0f, 0.0f); // Black  (top)
        glVertex2f(-1.0f, 1.0f);
        glVertex2f(1.0f, 1.0f);
        glColor3f(0.0f, 0.0f, 0.2f); //  dark blue (bottom)
        glVertex2f(1.0f, -1.0f);
        glVertex2f(-1.0f, -1.0f);

        // Draw stars
        glColor3f(1.0f, 1.0f, 1.0f); // White stars
        for (int i = 0; i < 25; i++) { // 100 stars
            float x = (rand() % 2000 - 1000) / 1000.0f; // Random x position
            float y = (rand() % 2000 - 1000) / 1000.0f; // Random y position
            glBegin(GL_POINTS);
            glVertex2f(x, y); // Draw each star
            glEnd();
        }
    }
    glEnd();
}


void drawMoonAndStars() {
    if (!dayNight) { // Night Mode
        // Draw Moon
        glColor3f(0.9f, 0.9f, 0.8f); // Light yellowish moon color
        glBegin(GL_TRIANGLE_FAN);
        glVertex2f(0.5f, 0.7f); // Moon center
        for (int i = 0; i <= 360; i++) {
            float angle = i * 3.14159f / 180.0f;
            float x = 0.1f * cos(angle) + 0.5f; // Moon radius 0.1
            float y = 0.1f * sin(angle) + 0.7f;
            glVertex2f(x, y);
        }
        glEnd();
    }
}


// Draw the buildings
void drawBuildings() {
    float x = -1.0f;   // Starting position for buildings
    float width = 0.15f; // Width of each building

    for (int i = 0; i < 10; ++i) {
        float height = 0.3f + 0.1f * (i % 3); // Vary building heights

        // Draw building structure
        setColor("building");
        glBegin(GL_QUADS);
        glVertex2f(x, -0.3f);
        glVertex2f(x + width, -0.3f);
        glVertex2f(x + width, height);
        glVertex2f(x, height);
        glEnd();

        // Draw windows
        setColor("buildingGlass");
        float windowStartX = x + 0.01f;
        float windowStartY = -0.1f;
        float windowWidth = 0.03f;
        float windowHeight = 0.09f;
        float gapX = 0.049f;
        float gapY = 0.1f;

        for (float wy = windowStartY; wy < height - 0.05f; wy += gapY) { // Fill building height with windows
            float wx = windowStartX;
            for (int k = 0; k < 3; ++k) { // 3 windows per row
                glBegin(GL_QUADS);
                glVertex2f(wx, wy);
                glVertex2f(wx + windowWidth, wy);
                glVertex2f(wx + windowWidth, wy + windowHeight);
                glVertex2f(wx, wy + windowHeight);
                glEnd();
                wx += gapX; // Move to next window in row
            }
        }
        x += width + 0.05f; // Move to next building
    }
}


// Draw the bridge
void drawBridge() {
    // Bridge base
    setColor("bridge");
    glBegin(GL_QUADS);
    glVertex2f(-1.0f, -0.3f);
    glVertex2f(1.0f, -0.3f);
    glVertex2f(1.0f, -0.25f);
    glVertex2f(-1.0f, -0.25f);
    glEnd();

    // Railings
    setColor("light");
    float railX = -1.0f;
    while (railX < 1.0f) {
        glBegin(GL_QUADS);
        glVertex2f(railX, -0.25f);
        glVertex2f(railX + 0.02f, -0.25f);
        glVertex2f(railX + 0.02f, -0.2f);
        glVertex2f(railX, -0.2f);
        glEnd();
        railX += 0.1f;
    }

    // Diagonal supports
    setColor("bridge");
    float supportX = -0.95f;
    while (supportX < 1.0f) {
        glBegin(GL_LINES);
        glVertex2f(supportX, -0.3f);           // Bottom of the bridge
        glVertex2f(supportX + 0.1f, -0.25f);  // Connect to the rail
        glEnd();

        glBegin(GL_LINES);
        glVertex2f(supportX + 0.1f, -0.3f);   // Bottom of the bridge
        glVertex2f(supportX, -0.25f);         // Connect to the rail
        glEnd();

        supportX += 0.2f;
    }

    // Vertical supports under the bridge
    float pillarX = -0.9f;
    while (pillarX < 1.0f) {
        glBegin(GL_QUADS);
        glVertex2f(pillarX, -0.3f);
        glVertex2f(pillarX + 0.05f, -0.3f);
        glVertex2f(pillarX + 0.05f, -0.6f);
        glVertex2f(pillarX, -0.6f);
        glEnd();
        pillarX += 0.2f;
    }
}

// Draw the road
void drawRoad() {
    // Draw the main road base
    setColor("road");
    glBegin(GL_QUADS);
    glVertex2f(-1.0f, -1.0f);
    glVertex2f(1.0f, -1.0f);
    glVertex2f(1.0f, -0.6f);
    glVertex2f(-1.0f, -0.6f);
    glEnd();

    // Draw sidewalks
    glColor3f(0.4f, 0.4f, 0.4f); // Gray sidewalk
    glBegin(GL_QUADS);
    glVertex2f(-1.0f, -0.6f);
    glVertex2f(1.0f, -0.6f);
    glVertex2f(1.0f, -0.55f);
    glVertex2f(-1.0f, -0.55f);
    glEnd();

    glBegin(GL_QUADS);
    glVertex2f(-1.0f, -1.05f);
    glVertex2f(1.0f, -1.05f);
    glVertex2f(1.0f, -1.0f);
    glVertex2f(-1.0f, -1.0f);
    glEnd();

    // Add lane markings (dashed white lines)
    setColor("laneMarking");
    float stripeWidth = 0.05f;
    float stripeSpacing = 0.15f;
    float stripeY = -0.8f;

    for (float x = -0.95f; x < 1.0f; x += stripeSpacing) {
        glBegin(GL_QUADS);
        glVertex2f(x, stripeY - stripeWidth / 2.0f);
        glVertex2f(x + 0.1f, stripeY - stripeWidth / 2.0f);
        glVertex2f(x + 0.1f, stripeY + stripeWidth / 2.0f);
        glVertex2f(x, stripeY + stripeWidth / 2.0f);
        glEnd();
    }

    // Add slight shading effect for depth
    glColor3f(0.15f, 0.15f, 0.15f);
    glBegin(GL_QUADS);
    glVertex2f(-1.0f, -0.85f);
    glVertex2f(1.0f, -0.85f);
    glVertex2f(1.0f, -0.80f);
    glVertex2f(-1.0f, -0.80f);
    glEnd();
}


void drawQuad(float x1, float y1, float x2, float y2) {
    glBegin(GL_QUADS);
    glVertex2f(x1, y1);
    glVertex2f(x2, y1);
    glVertex2f(x2, y2);
    glVertex2f(x1, y2);
    glEnd();
}
// Helper function to draw a circle (for wheels, headlights)
void drawCircle(float cx, float cy, float radius, int segments) {
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(cx, cy); // Center of the circle
    for (int i = 0; i <= segments; i++) {
        float angle = 2.0f * PI * i / segments;
        float x = radius * cos(angle);
        float y = radius * sin(angle);
        glVertex2f(cx + x, cy + y);
    }
    glEnd();
}


// Draw the train
void drawTrain() {
    glPushMatrix();
    glTranslatef(trainPosition, 0.0f, 0.0f);

    // Constants
    const float trainStartX = 0.7f;
    const float trainEndX = -0.7f;
    const float trainBodyY1 = -0.2f, trainBodyY2 = -0.1f;
    const float stripeY1 = -0.16f, stripeY2 = -0.15f;
    const float windowStartX = -0.55f, windowWidth = 0.1f, windowHeight = 0.03f;

    // Train body
    setColor("trainBody");
    drawQuad(trainStartX, trainBodyY1, trainEndX, trainBodyY2);

    // Train windows
    setColor("trainGlass");
    for (int i = 0; i < 10; ++i) {
        float x = windowStartX + i * 0.12f;
        drawQuad(x, -0.18f, x + windowWidth, -0.12f);
    }

    // Train stripe
    setColor("trainStripe");
    drawQuad(trainStartX, stripeY1, trainEndX, stripeY2);

    // Train front (triangle shape)
    setColor("trainFront");
    glBegin(GL_TRIANGLES);
    glVertex2f(trainEndX, trainBodyY1); // Bottom-left
    glVertex2f(trainEndX, trainBodyY2); // Top-left
    glVertex2f(trainEndX - 0.1f, (trainBodyY1 + trainBodyY2) / 1.5f); // Tip of the triangle
    glEnd();

    // Train wheels
    setColor("wheel");
    drawCircle(-0.4f, -0.22f, 0.05f, 20); // Left wheel
    drawCircle(0.4f, -0.22f, 0.05f, 20);  // Right wheel
    drawCircle(-0.6f, -0.22f, 0.05f, 20); // Additional wheel (left)
    drawCircle(0.6f, -0.22f, 0.05f, 20);  // Additional wheel (right)
    drawCircle(0.0f, -0.22f, 0.05f, 20);
    glPopMatrix();
}

// Draw smoke
void drawSmoke() {
    setColor("light");
    float smokeX = trainPosition - 0.7f; // Position smoke at the train's front
    for (int i = 0; i < 3; ++i) {
        drawCircle(smokeX + i * 0.05f, smokeY + i * 0.05f, 0.02f + i * 0.01f, 20);
    }
}

// Update animation
void update(int value) {
    trainPosition -= trainSpeed; // Move train left
    if (trainPosition < -1.6f) trainPosition = 1.6f; // Reset position
    
    


    smokeY += 0.005f; // Move smoke upwards
    if (smokeY > 0.5f) smokeY = 0.0f; // Reset smoke position

    glutPostRedisplay();
    glutTimerFunc(16, update, 0); // Recur every 16ms (60fps)
}

// Display function
void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    drawGradientSky();
    drawBuildings();
    drawBridge();
    drawRoad();
    drawTrain();

    drawMoonAndStars();
    drawSmoke();// Add smoke animation

    glutSwapBuffers();
}

// Handle keyboard input
void handleKeyboard(unsigned char key, int x, int y) {
    if (key == 'd') dayNight = true; // Day mode
    if (key == 'n') dayNight = false; // Night mode
    glutPostRedisplay(); // Refresh the display
}

void handleMouse(int button, int state, int x, int y) {
    if (state == GLUT_DOWN) {
        switch (button) {
            case GLUT_RIGHT_BUTTON: // Right-click to reverse direction
                trainSpeed = -trainSpeed;
                std::cout << "Train direction reversed. Speed: " << trainSpeed << std::endl;
                break;
        }
    }
    glutPostRedisplay();
}

// Initialization function
void init() {
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(-1.0, 1.0, -1.0, 1.0); // Set 2D projection
}


// Main function
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Train Animation");
    
    init();
    glutDisplayFunc(display);
    glutKeyboardFunc(handleKeyboard);
    glutMouseFunc(handleMouse);
    glutTimerFunc(16, update, 0); // 16ms for ~60fps

    glutMainLoop();
    return 0;
}
