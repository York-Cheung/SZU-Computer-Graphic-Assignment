#include "./include/Angel.h"

#pragma comment(lib, "glew32.lib")

const vec3 WHITE(1.0, 1.0, 1.0);
const vec3 BLACK(0.0, 0.0, 0.0);
const vec3 RED(1.0, 0.0, 0.0);
const vec3 GREEN(0.0, 1.0, 0.0);
const vec3 BLUE(0.0, 0.0, 1.0);
const vec3 YELLOW(1.0, 1.0, 0.0);  // Yellow
const vec3 CYAN(0.0, 1.0, 1.0); // Cyan
const vec3 MAGENTA(1.0, 0.0, 1.0);  // Magenta

const int CIRCLE_NUM_POINTS = 100;
const int ELLIPSE_NUM_POINTS = 100;

const int TRIANGLE_NUM_POINTS = 3;
const int SQUARE_NUM = 6;
const int SQUARE_NUM_POINTS = 4 * SQUARE_NUM;
const int TOTAL_NUM_POINTS = TRIANGLE_NUM_POINTS + SQUARE_NUM_POINTS + CIRCLE_NUM_POINTS + ELLIPSE_NUM_POINTS;

// ���Բ�ϵĵ�
vec2 getEllipseVertex(vec2 center, double scale, double verticalScale, double angle)
{
	vec2 vertex(sin(angle), cos(angle));
	vertex += center;
	vertex *= scale;
	vertex.y *= verticalScale;
	return vertex;
}

// ���ݽǶ�������ɫ
float generateAngleColor(double angle)
{
	return 1.0 / (2 * M_PI) * angle;
}

// ��������ε�ÿ���Ƕ�
double getTriangleAngle(int point)
{
	return 2 * M_PI / 3 * point;
}

// ��������ε�ÿ���Ƕ�
double getSquareAngle(int point)
{
	return M_PI / 4 + (M_PI / 2 * point);
}

void generateEllipsePoints(vec2 vertices[], vec3 colors[], int startVertexIndex, int numPoints,
	vec2 center, double scale, double verticalScale)
{
	double angleIncrement = (2 * M_PI) / numPoints;
	double currentAngle = M_PI / 2;

	for (int i = startVertexIndex; i < startVertexIndex + numPoints; ++i) {
		vertices[i] = getEllipseVertex(center, scale, verticalScale, currentAngle);
		if (verticalScale == 1.0) {
			colors[i] = vec3(generateAngleColor(currentAngle), 0.1, 0.5);
		} else {
			colors[i] = vec3(generateAngleColor(currentAngle), 0.1, 0.5);
		}
		currentAngle += angleIncrement;
	}
}

void generateTrianglePoints(vec2 vertices[], vec3 colors[], int startVertexIndex)
{
	double scale = 0.25;
	vec2 center(0.0, 0.70);

	for (int i = 0; i < 3; ++i) {
		double currentAngle = getTriangleAngle(i) + 0.5*M_PI;
		vertices[startVertexIndex + i] = vec2(sin(currentAngle), cos(currentAngle)) * scale + center;
	}

	colors[startVertexIndex] = YELLOW;
	colors[startVertexIndex + 1] = CYAN;
	colors[startVertexIndex + 2] = MAGENTA;
}

void generateSquarePoints(vec2 vertices[], vec3 colors[], int squareNumber, int startVertexIndex)
{
	double scale = 0.60;
	double scaleDecrease = 0.1;
	vec2 center(0.0, -0.25);
	int vertexIndex = startVertexIndex;

	for (int i = 0; i < squareNumber; ++i) {
		vec3 currentColor;
		currentColor = (i % 2) ? BLACK : YELLOW;
		for (int j = 0; j < 4; ++j) {
			double currentAngle = getSquareAngle(j);
			vertices[vertexIndex] = vec2(sin(currentAngle), cos(currentAngle)) * scale + center;
			colors[vertexIndex] = currentColor;
			vertexIndex++;
		}
		scale -= scaleDecrease;
	}
}

void init()
{
	vec2 vertices[TOTAL_NUM_POINTS];
	vec3 colors[TOTAL_NUM_POINTS];

	// ���ɸ�����״�ϵĵ�
	generateTrianglePoints(vertices, colors, 0);
	generateSquarePoints(vertices, colors, SQUARE_NUM, TRIANGLE_NUM_POINTS);
	/*����Բ�κ���Բ�ϵĵ����ɫ*/
	generateEllipsePoints(vertices, colors, TRIANGLE_NUM_POINTS + SQUARE_NUM_POINTS, CIRCLE_NUM_POINTS, vec2(-3,3), 0.2, 1);
	generateEllipsePoints(vertices, colors, TRIANGLE_NUM_POINTS + SQUARE_NUM_POINTS + CIRCLE_NUM_POINTS, ELLIPSE_NUM_POINTS, vec2(3,6), 0.2, 0.5);


	// ���������������
	GLuint vao[1];
	glGenVertexArrays(1, vao);
	glBindVertexArray(vao[0]);

	// ��������ʼ�����㻺�����
	GLuint buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices) + sizeof(colors), NULL, GL_STATIC_DRAW);

	// �ֱ��ȡ����
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertices), sizeof(colors), colors);

	// ��ȡ��ɫ����ʹ��
	GLuint program = InitShader("vshader.glsl", "fshader.glsl");
	glUseProgram(program);

	// �Ӷ�����ɫ���г�ʼ�������λ��
	GLuint pLocation = glGetAttribLocation(program, "vPosition");
	glEnableVertexAttribArray(pLocation);
	glVertexAttribPointer(pLocation, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
	// ��ƬԪ��ɫ���г�ʼ���������ɫ
	GLuint cLocation = glGetAttribLocation(program, "vColor");
	glEnableVertexAttribArray(cLocation);
	glVertexAttribPointer(cLocation, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(vertices)));

	// ��ɫ����
	glClearColor(1.0, 1.0, 1.0, 1.0);
}

void display(void)
{
	// ������
	glClear(GL_COLOR_BUFFER_BIT);

	// ����������
	glDrawArrays(GL_TRIANGLES, 0, TRIANGLE_NUM_POINTS);
	// ���ƶ��������
	for (int i = 0; i < SQUARE_NUM; ++i) {
		glDrawArrays(GL_TRIANGLE_FAN, TRIANGLE_NUM_POINTS + (i * 4), 4);
	}

	// ����Բ����Բ
	glDrawArrays(GL_POLYGON, SQUARE_NUM_POINTS + TRIANGLE_NUM_POINTS, CIRCLE_NUM_POINTS);
	glDrawArrays(GL_POLYGON, SQUARE_NUM_POINTS + TRIANGLE_NUM_POINTS + CIRCLE_NUM_POINTS, ELLIPSE_NUM_POINTS);

	// �����������ߺ���������
	GLfloat x;
	glBegin(GL_LINE_STRIP);
	for (x = -1.0f / 0.1f; x<1.0f / 0.1f; x += 0.01f)
	{
		glVertex2f(x*0.1f, (sin(x)*0.1f) - 0.8);

	}
	glEnd();

	GLfloat x1;
	glBegin(GL_LINE_STRIP);
	for (x1 = -1.0f / 0.1f; x1<1.0f / 0.1f; x1 += 0.01f)
	{
		glVertex2f(x1*0.1f, (cos(x1)*0.1f) - 0.8);
	}
	glEnd();

	// ���������
    glColor3f(1.0f, 0.0f, 0.0f);//��ɫ�������
	GLfloat a = 1 / (4 - 4 * cos(72 * M_PI / 180));
	GLfloat bx = a * cos(18 * M_PI / 180);
	GLfloat by = a * sin(18 * M_PI / 180);
	GLfloat cy = -a * cos(18 * M_PI / 180);
	GLfloat
		PointA[2] = { 0 + 0.7, a  },
		PointB[2] = { bx + 0.7, by  },
		PointC[2] = { 0.25 + 0.7, cy },
		PointD[2] = { -0.25 + 0.7, cy  },
		PointE[2] = { -bx + 0.7, by  };  //���������ҷ�ƽ��
	// ����A->C->E->B->D->A��˳�򣬽�����ǻ���
	glBegin(GL_LINE_LOOP);//�պ�����
	glVertex2fv(PointA);
	glVertex2fv(PointC);
	glVertex2fv(PointE);
	glVertex2fv(PointB);
	glVertex2fv(PointD);
	glEnd();


	glFlush();
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA);
	glutInitWindowSize(590, 500);
	glutCreateWindow("2015160180_��Ծ_ʵ��һ");

	glewExperimental = GL_TRUE;
	glewInit();

	init();
	glutDisplayFunc(display);

	glutMainLoop();
	return 0;
}
