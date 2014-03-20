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
#include <pthread.h>

void junk() {
	int i;
	i = pthread_getconcurrency();
	i++;
}

unsigned int reso_x = 800, reso_y = 600; // Window size in pixels
const float world_x = 8.f, world_y = 6.f; // Level (world) size in meters

int last_time;
int frame_count;
int play = 0;

// Information about the levels loaded from files will be available in these.
unsigned int num_levels;
level_t *levels;

b2World *world;
b2Body *ground;
b2Body *ball;
unsigned int current_level;
level_t *cur_level;

float ball_radius = 0.1F;
float finish_width = 0.05F;
float finish_height = 0.05F;

/* Support at most 4 vertices, where the last vertice more or less equals the first. */
// ^Extreme bullshit alert... Bij een driehoek geef je ook maar drie hoekpunten mee...^
const int max_vertices = 4;
int mouse_mode = 0;

b2Vec2 draw_vertices[max_vertices];
int num_vertices = -1;

void makePolygon(int is_dynamic, float pos_x, float pos_y, b2Vec2 *vertices, int vertices_amount) {
	b2BodyDef body_def;

	if (is_dynamic) {
		body_def.type = b2_dynamicBody;
	}

	body_def.position.Set(pos_x, pos_y);

	b2Body *body = world->CreateBody(&body_def);

	b2PolygonShape polygon;
	polygon.Set(vertices, vertices_amount);

	b2FixtureDef fixture_def;
	fixture_def.shape = &polygon;
	fixture_def.density = 1.0f;
	fixture_def.friction = 0.3f;
	body->CreateFixture(&fixture_def);
}

/*
 * Load a given world, i.e. read the world from the `levels' data structure and
 * convert it into a Box2D world.
 */
void load_world(unsigned int level) {
	// Pause the game when loading a new level
	play = 0;

	if (level >= num_levels) {
		// Note that level is unsigned but we still use %d so -1 is shown as
		// such.
		printf("Warning: level %d does not exist.\n", level);
		return;
	}

	// Create a Box2D world and populate it with all bodies for this level
	// (including the ball).
	current_level = level + 1;
	cur_level = &(levels[level]);
	b2Vec2 gravity (0, -9.81);
	bool do_sleep = true;

	world = new b2World(gravity);
	world->SetAllowSleeping(do_sleep);

	// printf("num_polygons = %i\n", levels[0].num_polygons);
	// printf("is_Dynamic = %d\n", levels[0].polygons[0].is_dynamic);

	// Create ground
	b2BodyDef groundBodyDef;
	groundBodyDef.position.Set(0.0f, -world_y / 2);
	ground = world->CreateBody(&groundBodyDef);

	b2PolygonShape groundBox;
	groundBox.SetAsBox(world_x, world_y / 2);
	ground->CreateFixture(&groundBox, 0.0f);

	// Create ball
	b2BodyDef ballBodyDef;
	ballBodyDef.type = b2_dynamicBody;
	ballBodyDef.position.Set(cur_level->start.x, cur_level->start.y);
	ball = world->CreateBody(&ballBodyDef);

	b2CircleShape dynamicCircle;
	dynamicCircle.m_radius = ball_radius;

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &dynamicCircle;
	fixtureDef.density = 1.0f;
	fixtureDef.friction = 0.3f;
	ball->CreateFixture(&fixtureDef);

	// Setup rest of level.
	for (unsigned int i = 0; i < cur_level->num_polygons; i++) {
		b2Vec2 *vertices = new b2Vec2[cur_level->polygons[i].num_verts];

		for (unsigned int j = 0; j < cur_level->polygons[i].num_verts; j ++) {
			vertices[j].Set(cur_level->polygons[i].verts[j].x, cur_level->polygons[i].verts[j].y);
		}

		makePolygon(cur_level->polygons[i].is_dynamic, cur_level->polygons[i].position.x,
			cur_level->polygons[i].position.y, vertices, cur_level->polygons[i].num_verts);

		delete[] vertices;
	}
}

float calcDistance(float x1, float y1, float x2, float y2) {
	return (x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2);
}

/* Returns true if the ball is on the edge of the ball, or inside the ball. */
bool ballHasReachedFinish(b2Vec2 ball_position) {
	float finish_x = levels[current_level].end.x;
	float finish_y = levels[current_level].end.y;
	float ball_x = ball_position.x;
	float ball_y = ball_position.y;

	/* Calculate the distances between the 4 corners of the finish square and the center of the ball. */
	float distance1 = calcDistance(finish_x, finish_y, ball_x, ball_y);
	float distance2 = calcDistance(finish_x, finish_y, ball_x, ball_y);
	float distance3 = calcDistance(finish_x, finish_y, ball_x, ball_y);
	float distance4 = calcDistance(finish_x, finish_y, ball_x, ball_y);

	/* If one of them is lower than the radius of the ball, the ball has reached the finish. */
	return ((distance1 <= ball_radius) || (distance2 <= ball_radius) ||
		(distance3 <= ball_radius) || (distance4 <= ball_radius));
}

void drawPolygon(b2Body *body, b2PolygonShape *polygon) {
	glColor3f(0.0, 1.0, 0.0);
	glBegin(GL_POLYGON);

	for (int i = 0; i < polygon->GetVertexCount(); i++) {
		b2Vec2 vertex = body->GetWorldPoint(polygon->GetVertex(i));
		glVertex2f(vertex.x, vertex.y);
	}

	glEnd();
}

void drawBall() {
	b2Vec2 position = ball->GetPosition();

	// Draw the ball using a triangle fan so it looks sphere-like.
	glColor3f(1.0, 0.0, 0.0);
	glBegin(GL_TRIANGLE_FAN);
	glVertex2f(position.x, position.y);

	for (int i = 0; i <= 360; i += 5) {
		glVertex2f(position.x + sin(i) * ball_radius, position.y + cos(i) * ball_radius);
	}

	glEnd();
}

void drawFinish() {
	point_t finish = levels[current_level].end;

	glColor3f(0.0, 0.0, 1.0);
	glBegin(GL_POLYGON);
		glVertex2f(finish.x - finish_width, finish.y - finish_height);
		glVertex2f(finish.x - finish_width, finish.y + finish_height);
		glVertex2f(finish.x + finish_width, finish.y + finish_height);
		glVertex2f(finish.x + finish_width, finish.y - finish_height);
	glEnd();
}

void drawVertex(int amount) {
	glColor3f(1.0, 1.0, 1.0);
	for (int i = 0; i <= amount; ++i) {
		glBegin(GL_POLYGON);
			glVertex2f(draw_vertices[i].x - 0.05F, draw_vertices[i].y - 0.05F);
			glVertex2f(draw_vertices[i].x - 0.05F, draw_vertices[i].y + 0.05F);
			glVertex2f(draw_vertices[i].x + 0.05F, draw_vertices[i].y + 0.05F);
			glVertex2f(draw_vertices[i].x + 0.05F, draw_vertices[i].y - 0.05F);
		glEnd();
	}
}

/*
 * Called when we should redraw the scene (i.e. every frame).
 * It will show the current framerate in the window title.
 */
void draw(void) {
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

	b2Body *body_list = world->GetBodyList();
	int current_polygon = 0;

	while(body_list != NULL) {
		b2Fixture *fixture_list = body_list->GetFixtureList();

		if (ground == body_list) {
			body_list = body_list->GetNext();
			current_polygon++;
			continue;
		}

		while(fixture_list != NULL) {
			if (fixture_list->GetType() == 2) {
				b2PolygonShape *poly = (b2PolygonShape*) fixture_list->GetShape();
				drawPolygon(body_list, poly);
			}

			fixture_list = fixture_list->GetNext();
		}

		body_list = body_list->GetNext();
		current_polygon++;
	}

	if (play) {
		world->Step(timestep, velocity_iterations, position_iterations);

		/* If the ball has reached the finish, continue to the next level or finish the game. */
		b2Vec2 position = ball->GetPosition();

		if (ballHasReachedFinish(position)) {
			if (current_level > num_levels) {
				fprintf(stderr, "You have won the game! Also, you have lost the game... ;)\n");
				exit(0);
			} else {
				fprintf(stderr, "You finished level %d!\n", current_level);

				if (current_level >= num_levels) {
					fprintf(stderr, "You have won the game! Also, you have lost the game... ;)\n");
					exit(0);
				} else {
					load_world(current_level);

					/* Automatically pause the world... */
					play = 0;
				}
			}
		}
	}

	// Draw ball
	drawBall();

	// Draw finish
	drawFinish();

	if (num_vertices > -1) {
		drawVertex(num_vertices);
		if (num_vertices == max_vertices - 1) {
			num_vertices = -1;
		}
	}

	// Show rendered frame
	glutSwapBuffers();

	// Display fps in window title.
	if (frametime >= 1000) {
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
void resize_window(int width, int height) {
	glViewport(0, 0, width, height);
	reso_x = width;
	reso_y = height;
}

/*
 * Called when the user presses a key.
 */
void key_pressed(unsigned char key, int x, int y) {
	switch (key) {
		case 27: // Esc
		case 'q':
			exit(0);
			break;
		// Add any keys you want to use, either for debugging or gameplay.
		case ' ':
			play = !play;
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
void mouse_clicked(int button, int state, int x, int y) {
	// guard against both left and right buttons being pressed at the same time,
	// by only responding when a mouse button is pressed while another one
	// hasn't been pressed yet
	if (state == GLUT_DOWN && mouse_mode == 0) {
		if (button == GLUT_LEFT_BUTTON) {
			mouse_mode = GLUT_LEFT_BUTTON;
			num_vertices++;

			/* Add the mouse click to the vertices array: determine cases... */

			/* Convert clicked mouse point to world vertex. */
			draw_vertices[num_vertices].x = x * (world_x / reso_x);
			draw_vertices[num_vertices].y = (reso_y - y) * (world_y / reso_y);

			/* First case: attempt at creating a quad... */
			if (num_vertices == max_vertices - 1) {
				makePolygon(1, 0, 0, draw_vertices, max_vertices);
			}
			// printf("num_vertices = %d, max_vertices = %d\n", num_vertices, max_vertices);
		} else if (button == GLUT_RIGHT_BUTTON) {
			mouse_mode = GLUT_RIGHT_BUTTON;

			/* Possible TODO: Remove the point? */
		}
	} else if (state == GLUT_UP && button == mouse_mode) {
		// pressed button released
		mouse_mode = 0;
	}
}

/*
 * Called when the mouse is moved to a certain given position.
 */
void mouse_moved(int x, int y) {

}


int main(int argc, char **argv) {
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
