/* Computer Graphics and Game Technology, Assignment Box2D game
 *
 * Student name ....
 * Student email ...
 * Collegekaart ....
 * Date ............
 * Comments ........
 *
 *
 * (always fill in these fields before submitting!!)
 */

#include <cstdio>

#include <GL/gl.h>
#include <GL/glut.h>
#include <Box2D/Box2D.h>

#include "levels.h"

unsigned int reso_x = 800, reso_y = 600; // Window size in pixels
const float world_x = 8.f, world_y = 6.f; // Level (world) size in meters

int last_time;
int frame_count;
int play = 0;

// Information about the levels loaded from files will be available in these.
unsigned int num_levels;
level_t *levels;

b2World* world;
b2Body* body;
int current_level;

/*
 * Load a given world, i.e. read the world from the `levels' data structure and
 * convert it into a Box2D world.
 */
void load_world(unsigned int level) {

    if (level >= num_levels)
    {
        // Note that level is unsigned but we still use %d so -1 is shown as
        // such.
        printf("Warning: level %d does not exist.\n", level);
        return;
    }

    // Create a Box2D world and populate it with all bodies for this level
    // (including the ball).
    current_level = level + 1;
    b2Vec2 gravity (0, -9.81);
    bool do_sleep = true;

    world = new b2World(gravity);
    world->SetAllowSleeping(do_sleep);

    // printf("num_polygons = %i\n", levels[0].num_polygons);
    // printf("is_Dynamic = %d\n", levels[0].polygons[0].is_dynamic);

    // Setup ball.
    b2BodyDef body_def;
    body_def.type = b2_dynamicBody;
    body_def.position.Set(levels[level].start.x, levels[level].start.y);
    body = world->CreateBody(&body_def);

    b2CircleShape circle;
    // circle.m_p.Set(3.0f, 4.0f);
    circle.m_radius = 0.5f;

    b2FixtureDef fixture_def;
    fixture_def.shape = &circle;
    fixture_def.density = 1.0f;
    fixture_def.friction = 0.3f;

    body->CreateFixture(&fixture_def);
    b2Vec2 triangle[3];

    // Setup rest of level.
    // for (int i = 0; i < levels[level].num_polygons; i++) {
    //     [levels[level].polygons[i].num_verts];
    //     triangle
    // }
}


/*
 * Called when we should redraw the scene (i.e. every frame).
 * It will show the current framerate in the window title.
 */
void draw(void)
{
    int time = glutGet(GLUT_ELAPSED_TIME);
    int frametime = time - last_time;
    frame_count++;

    // Clear the buffer
    glColor3f(0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT);


    //
    // Do any logic and drawing here.
    //

    // suggested vel = 8, suggested pos = 3.
    float32 timestep = 1.0f / 60.0f;
    int32 velocity_iterations = 6;
    int32 position_iterations = 2;
    if (play) {
        world->Step(timestep, velocity_iterations, position_iterations);
    }

    b2Vec2 position = body->GetPosition();

    // Draw circle
    glColor3f(1.0, 0.0, 0.0);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(position.x, position.y);
    for (int i = 0; i <= 360; i += 5) {
        glVertex2f(position.x + sin(i) * 0.1, position.y + cos(i) * 0.1);
    }
    glEnd();

    // Show rendered frame
    glutSwapBuffers();

    // Display fps in window title.
    if (frametime >= 1000)
    {
        char window_title[128];
        snprintf(window_title, 128,
                "Box2D: %f fps, level %d/%d",
                frame_count / (frametime / 1000.f), current_level, num_levels);
        glutSetWindowTitle(window_title);
        last_time = time;
        frame_count = 0;
    }
}

/*
 * Called when window is resized. We inform OpenGL about this, and save this
 * for future reference.
 */
void resize_window(int width, int height)
{
    glViewport(0, 0, width, height);
    reso_x = width;
    reso_y = height;
}

/*
 * Called when the user presses a key.
 */
void key_pressed(unsigned char key, int x, int y)
{
    switch (key)
    {
        case 27: // Esc
        case 'q':
            exit(0);
            break;
        // Add any keys you want to use, either for debugging or gameplay.
        case ' ':
            if (play) {
                play = 0;
            }
            else {
                play = 1;
            }
            break;
        case '1':
            load_world(0);
            break;
        case '2':
            load_world(1);
            break;
        case '3':
            load_world(2);
            break;
        case '4':
            load_world(3);
            break;
        case '5':
            load_world(4);
            break;
        default:
            break;
    }
}

/*
 * Called when the user clicked (or released) a mouse buttons inside the window.
 */
void mouse_clicked(int button, int state, int x, int y)
{

}

/*
 * Called when the mouse is moved to a certain given position.
 */
void mouse_moved(int x, int y)
{

}


int main(int argc, char **argv)
{
    // Create an OpenGL context and a GLUT window.
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(reso_x, reso_y);
    glutCreateWindow("Box2D");

    // Bind all GLUT events do callback function.
    glutDisplayFunc(&draw);
    glutIdleFunc(&draw);
    glutReshapeFunc(&resize_window);
    glutKeyboardFunc(&key_pressed);
    glutMouseFunc(&mouse_clicked);
    glutMotionFunc(&mouse_moved);
    glutPassiveMotionFunc(&mouse_moved);

    // Initialise the matrices so we have an orthogonal world with the same size
    // as the levels, and no other transformations.
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, world_x, 0, world_y, 0, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Read the levels into a bunch of structs.
    num_levels = load_levels(&levels);
    printf("Loaded %d levels.\n", num_levels);

    // Load the first level (i.e. create all Box2D stuff).
    load_world(0);

    last_time = glutGet(GLUT_ELAPSED_TIME);
    frame_count = 0;
    glutMainLoop();

    return 0;
}
