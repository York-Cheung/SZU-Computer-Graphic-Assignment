#include "./include/Angel.h"

#include <vector>
#include <string>
#include <fstream>

#pragma comment(lib, "glew32.lib")

// ������Ƭ�еĶ�������(���������б�)
typedef struct vIndex {
	unsigned int a, b, c;
	vIndex(int ia, int ib, int ic) : a(ia), b(ib), c(ic) {}
} vec3i;

std::string filename;
std::vector<vec3> vertices;
std::vector<vec3i> faces;

int nVertices = 0;
int nFaces = 0;
int nEdges = 0;

std::vector<vec3> points;   //������ɫ���Ļ��Ƶ�
std::vector<vec3> colors;   //������ɫ������ɫ

const int NUM_VERTICES = 8;

const vec3 vertex_colors[NUM_VERTICES] = {
	vec3(1.0, 1.0, 1.0),  // White
	vec3(1.0, 1.0, 0.0),  // Yellow
	vec3(0.0, 1.0, 0.0),  // Green
	vec3(0.0, 1.0, 1.0),  // Cyan
	vec3(1.0, 0.0, 1.0),  // Magenta
	vec3(1.0, 0.0, 0.0),  // Red
	vec3(0.0, 0.0, 0.0),  // Black
	vec3(0.0, 0.0, 1.0)   // Blue
};

void read_off(const std::string filename)
{
	if (filename.empty()) {
		return;
	}
	std::ifstream fin;
	fin.open(filename);
	// @TODO: �޸Ĵ˺�����ȡOFF�ļ�����άģ�͵���Ϣ
	// ��ȡOFF�ļ������洢��Ϣ���ⲿ������
	std::string off_sign;
	fin >> off_sign;
	fin >> nVertices >> nFaces >> nEdges;
	std::cout << "��������:" << nVertices
			  << "������:" << nFaces
			  << "������:" << nEdges << std::endl;
	//���㰴ÿ��һ���г�x��y��z���ꡣ
	for (int i = 0; i < nVertices; i++) {
		double x, y, z;
		fin >> x >> y >> z;
		vertices.push_back(vec3(x, y, z));
	}
	//�ڶ����б����Ƭ����ÿ��һ���б�����ÿ����Ƭ�������������ָ���ģ��������Ƕ��������б�
	for (int i = 0; i < nFaces; i++)
	{
		int num, a, b, c;
		fin >> num >> a >> b >> c;
		faces.push_back(vec3i(a, b, c));
	}

	fin.close();
}

void storeFacesPoints()
{
	points.clear();
	colors.clear();
	// @TODO: �޸Ĵ˺�����points��colors�����д洢ÿ��������Ƭ�ĸ��������ɫ��Ϣ
	int faces_size = faces.size();
	for (int i = 0; i < faces_size; i++)
	{
		points.push_back(vertices[faces[i].a]);
		points.push_back(vertices[faces[i].b]);
		points.push_back(vertices[faces[i].c]);
	}
	for (int i = 0; i < faces_size; i++)
	{
		colors.push_back(vertex_colors[faces[i].a]);
		colors.push_back(vertex_colors[faces[i].b]);
		colors.push_back(vertex_colors[faces[i].c]);
	}

}

void init()
{
	storeFacesPoints();

	// ���������������
	GLuint vao[1];
	glGenVertexArrays(1, vao);
	glBindVertexArray(vao[0]);

	// ��������ʼ�����㻺�����
	GLuint buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, points.size() * sizeof(vec3) + colors.size() * sizeof(vec3), NULL, GL_STATIC_DRAW);

	// @TODO: �޸���ɺ��ٴ�����ע�ͣ��������ᱨ��
	// �ֱ��ȡ����
	glBufferSubData(GL_ARRAY_BUFFER, 0, points.size() * sizeof(vec3), &points[0]);
	glBufferSubData(GL_ARRAY_BUFFER, points.size() * sizeof(vec3), colors.size() * sizeof(vec3), &colors[0]);

	// ��ȡ��ɫ����ʹ��
	GLuint program = InitShader("vshader.glsl", "fshader.glsl");
	glUseProgram(program);

	// �Ӷ�����ɫ���г�ʼ�������λ��
	GLuint pLocation = glGetAttribLocation(program, "vPosition");
	glEnableVertexAttribArray(pLocation);
	glVertexAttribPointer(pLocation, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
	// ��ƬԪ��ɫ���г�ʼ���������ɫ
	GLuint cLocation = glGetAttribLocation(program, "vColor");
	glEnableVertexAttribArray(cLocation);
	glVertexAttribPointer(cLocation, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(points.size() * sizeof(vec3)));

	// ��ɫ����
	glClearColor(0.0, 0.0, 0.0, 1.0);
}

void display(void)
{
	// @TODO: �����ڣ�������ɫ�������Ȼ���
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// ���Ʊ�
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	// �����������
	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_FRONT);

	glDrawArrays(GL_TRIANGLES, 0, points.size());
	glutSwapBuffers();
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	// @TODO: ������ʾģʽ֧����Ȳ���
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);

	glutInitWindowSize(600, 600);
	glutCreateWindow("3D OFF Model");

	glewExperimental = GL_TRUE;
	glewInit();

	// ��ȡoffģ���ļ�
	read_off("cube.off");

	init();
	glutDisplayFunc(display);

	// @TODO: ������Ȳ���
	glEnable(GL_DEPTH_TEST);

	glutMainLoop();
	return 0;
}
