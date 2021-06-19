#include "Renderer.h"
#include <vector>
#include <string>
#include <fstream>
#include <iostream>

using namespace std;

//���� Ŭ���� �����ؼ� vertex���� ����
class CPoint2f {
public:
	vector <float> d{ 0,0 };
};

class CPoint3f {
public:
	vector<float> d{ 0, 0, 0 };
};

class CPoint3i {
public:
	vector<int> d{ 0, 0, 0 };
};

class CFace {
public:
	vector<CPoint3i> v_pairs;
};

class CObj {
public:
	string name;
	vector<CPoint3f> v;
	vector<CPoint2f> vt;
	vector<CPoint3f> vn;
	vector<CFace> f;
};

class CModel {
public:
	vector<CObj> objs; //�������� obj ����


	//���ڸ� ���� ������ �и��ϴ� �Լ�
	vector<float> my_strtok_f(char* str, char* delimeter) {
		vector<float> v;
		char* context;
		char* tok = strtok_s(str, delimeter, &context);

		while (tok != NULL) {
			v.push_back(atof(tok));
			tok = strtok_s(context, delimeter, &context);
		}
		return v;
	}
	vector<string> my_strtok_s(char* str, char* delimeter) {
		vector<string> v;
		char* context;
		char* tok = strtok_s(str, delimeter, &context);

		while (tok != NULL) {
			v.push_back(tok);
			tok = strtok_s(context, delimeter, &context);
		}
		return v;
	}
	vector<int> my_strtok_i(char* str, char* delimeter) {
		vector<int> v;
		char* context;
		char* tok = strtok_s(str, delimeter, &context);

		while (tok != NULL) {
			v.push_back(atoi(tok));
			tok = strtok_s(context, delimeter, &context);
		}
		return v;
	}

	void loadObj(ifstream& fin) {
		string line;
		CObj obj_tmp;
		int cnt = 0;
		while (getline(fin, line)) {
			int len = line.length();
			vector<float> vf;
			vector<string> s;
			vector<int> vi;
			CPoint3f p3;
			CPoint2f p2;
			CPoint3i p3i;
			if (line[0] == 'o' && line[1] == ' ') { //str ���ϴ� �κ� ������ ��
				obj_tmp.name = line.substr(2, len - 2);
				objs.push_back(obj_tmp);
				cnt += 1;
			}

			if (line[0] == 'v' && line[1] == ' ') {
				vf = my_strtok_f((char*)line.substr(2, len - 2).c_str(), (char*)" "); //�� ���� ������ �Լ��� ����
				p3.d = { vf[0], vf[1], vf[2] };
				objs[cnt - 1].v.push_back(p3);
			}
			else if (line[0] == 'v' && line[1] == 't') {
				vf = my_strtok_f((char*)line.substr(3, len - 2).c_str(), (char*)" "); //�� ���� ������ �Լ��� ����
				p2.d = { vf[0], vf[1] };
				objs[cnt - 1].vt.push_back(p2);
			}
			else if (line[0] == 'v' && line[1] == 'n') {
				vf = my_strtok_f((char*)line.substr(3, len - 2).c_str(), (char*)" "); //�� ���� ������ �Լ��� ����
				p3.d = { vf[0], vf[1], vf[2] };
				objs[cnt - 1].vn.push_back(p3);
			}
			else if (line[0] == 'f' && line[1] == ' ') {
				vf = my_strtok_f((char*)line.substr(2, len - 2).c_str(), (char*)" "); //�� ���� ������ �Լ��� ����
				int nVertexes = s.size();
				CFace face_tmp;
				for (int i = 0; i < nVertexes; i++) {
					vi = my_strtok_i((char*)s[i].c_str(), (char*)"/");
					p3i.d = { vi[0], vi[1],vi[2] };
					face_tmp.v_pairs.push_back(p3i);
				}
				objs[cnt - 1].f.push_back(face_tmp);
			}
		}
	}
};

CModel models; //�𵨵� �ִ� Ŭ���� ����

void draw_center(void)
{
	glBegin(GL_LINES);
	glColor3f(1.0f, 0.0f, 0.0f); /* R */
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.2f, 0.0f, 0.0f);
	glEnd();
	glRasterPos3f(0.2f, 0.0f, 0.0f);
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'x');

	glBegin(GL_LINES);
	glColor3f(0.0f, 1.0f, 0.0f); /* G */
	glVertex3f(0.0f, 0.2f, 0.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glEnd();
	glRasterPos3f(0.0f, 0.2f, 0.0f);
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'y');

	glBegin(GL_LINES);
	glColor3f(0.0f, 0.0f, 1.0f); /* B */
	glVertex3f(0.0f, 0.0f, -0.2f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glEnd();
	glRasterPos3f(0.0f, 0.0f, -0.2f);
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'z');
}

void idle() {
	static GLuint previousClock = glutGet(GLUT_ELAPSED_TIME);
	static GLuint currentClock = glutGet(GLUT_ELAPSED_TIME);
	static GLfloat deltaT;

	currentClock = glutGet(GLUT_ELAPSED_TIME);
	deltaT = currentClock - previousClock;
	if (deltaT < 1000.0 / 20.0) { return; }
	else { previousClock = currentClock; }

	//char buff[256];
	//sprintf_s(buff, "Frame Rate = %f", 1000.0 / deltaT);
	//frameRate = buff;

	glutPostRedisplay();
}

void close()
{
	glDeleteTextures(1, &dispBindIndex);
	glutLeaveMainLoop();
	CloseHandle(hMutex);
}

void add_quats(float q1[4], float q2[4], float dest[4])
{
	static int count = 0;
	float t1[4], t2[4], t3[4];
	float tf[4];

	vcopy(q1, t1);
	vscale(t1, q2[3]);

	vcopy(q2, t2);
	vscale(t2, q1[3]);

	vcross(q2, q1, t3);
	vadd(t1, t2, tf);
	vadd(t3, tf, tf);
	tf[3] = q1[3] * q2[3] - vdot(q1, q2);

	dest[0] = tf[0];
	dest[1] = tf[1];
	dest[2] = tf[2];
	dest[3] = tf[3];

	if (++count > RENORMCOUNT) {
		count = 0;
		normalize_quat(dest);
	}
}

void reshape(int width, int height)
{
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(58, (double)width / height, 0.1, 100);
	glMatrixMode(GL_MODELVIEW);
}

void motion(int x, int y)
{
	GLfloat spin_quat[4];
	float gain;
	gain = 2.0; /* trackball gain */

	if (drag_state == GLUT_DOWN)
	{
		if (button_state == GLUT_LEFT_BUTTON)
		{
			trackball(spin_quat,
				(gain * rot_x - 500) / 500,
				(500 - gain * rot_y) / 500,
				(gain * x - 500) / 500,
				(500 - gain * y) / 500);
			add_quats(spin_quat, quat, quat);
		}
		else if (button_state == GLUT_RIGHT_BUTTON)
		{
			t[0] -= (((float)trans_x - x) / 500);
			t[1] += (((float)trans_y - y) / 500);
		}
		else if (button_state == GLUT_MIDDLE_BUTTON)
			t[2] -= (((float)trans_z - y) / 500 * 4);
		else if (button_state == 3 || button_state == 4) // scroll
		{

		}
		//glutPostRedisplay();
	}

	rot_x = x;
	rot_y = y;

	trans_x = x;
	trans_y = y;
	trans_z = y;
}

void mouse(int button, int state, int x, int y)
{
	if (state == GLUT_DOWN)
	{
		if (button == GLUT_LEFT_BUTTON)
		{
			rot_x = x;
			rot_y = y;

			//t[0] = t[0] + 1;


		}
		else if (button == GLUT_RIGHT_BUTTON)
		{
			trans_x = x;
			trans_y = y;
		}
		else if (button == GLUT_MIDDLE_BUTTON)
		{
			//trcon = trcon + 1;
			trans_z = y;
		}
		else if (button == 3 || button == 4)
		{
			const float sign = (static_cast<float>(button)-3.5f) * 2.0f;
			t[2] -= sign * 500 * 0.00015f;
		}
	}

	drag_state = state;
	button_state = button;
}

void vzero(float* v)
{
	v[0] = 0.0f;
	v[1] = 0.0f;
	v[2] = 0.0f;
}

void vset(float* v, float x, float y, float z)
{
	v[0] = x;
	v[1] = y;
	v[2] = z;
}

void vsub(const float *src1, const float *src2, float *dst)
{
	dst[0] = src1[0] - src2[0];
	dst[1] = src1[1] - src2[1];
	dst[2] = src1[2] - src2[2];
}

void vcopy(const float *v1, float *v2)
{
	register int i;
	for (i = 0; i < 3; i++)
		v2[i] = v1[i];
}

void vcross(const float *v1, const float *v2, float *cross)
{
	float temp[3];

	temp[0] = (v1[1] * v2[2]) - (v1[2] * v2[1]);
	temp[1] = (v1[2] * v2[0]) - (v1[0] * v2[2]);
	temp[2] = (v1[0] * v2[1]) - (v1[1] * v2[0]);
	vcopy(temp, cross);
}

float vlength(const float *v)
{
	return sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
}

void vscale(float *v, float div)
{
	v[0] *= div;
	v[1] *= div;
	v[2] *= div;
}

void vnormal(float *v)
{
	vscale(v, 1.0f / vlength(v));
}

float vdot(const float *v1, const float *v2)
{
	return v1[0] * v2[0] + v1[1] * v2[1] + v1[2] * v2[2];
}

void vadd(const float *src1, const float *src2, float *dst)
{
	dst[0] = src1[0] + src2[0];
	dst[1] = src1[1] + src2[1];
	dst[2] = src1[2] + src2[2];
}

void trackball(float q[4], float p1x, float p1y, float p2x, float p2y)
{
	float a[3]; /* Axis of rotation */
	float phi;  /* how much to rotate about axis */
	float p1[3], p2[3], d[3];
	float t;

	if (p1x == p2x && p1y == p2y) {
		/* Zero rotation */
		vzero(q);
		q[3] = 1.0;
		return;
	}

	/*
	 * First, figure out z-coordinates for projection of P1 and P2 to
	 * deformed sphere
	 */
	vset(p1, p1x, p1y, tb_project_to_sphere(TRACKBALLSIZE, p1x, p1y));
	vset(p2, p2x, p2y, tb_project_to_sphere(TRACKBALLSIZE, p2x, p2y));

	/*
	 *  Now, we want the cross product of P1 and P2
	 */
	vcross(p2, p1, a);

	/*
	 *  Figure out how much to rotate around that axis.
	 */
	vsub(p1, p2, d);
	t = vlength(d) / (2.0f*TRACKBALLSIZE);

	/*
	 * Avoid problems with out-of-control values...
	 */
	if (t > 1.0) t = 1.0;
	if (t < -1.0) t = -1.0;
	phi = 2.0f * asin(t);

	axis_to_quat(a, phi, q);
}

void axis_to_quat(float a[3], float phi, float q[4])
{
	vnormal(a);
	vcopy(a, q);
	vscale(q, sin(phi / 2.0f));
	q[3] = cos(phi / 2.0f);
}

float tb_project_to_sphere(float r, float x, float y)
{
	float d, t, z;

	d = sqrt(x*x + y*y);
	if (d < r * 0.70710678118654752440f) {    /* Inside sphere */
		z = sqrt(r*r - d*d);
	}
	else {           /* On hyperbola */
		t = r / 1.41421356237309504880f;
		z = t*t / d;
	}
	return z;
}

void normalize_quat(float q[4])
{
	int i;
	float mag;

	mag = (q[0] * q[0] + q[1] * q[1] + q[2] * q[2] + q[3] * q[3]);
	for (i = 0; i < 4; i++) q[i] /= mag;
}

void build_rotmatrix(float m[4][4], float q[4])
{
	m[0][0] = 1.0f - 2.0f * (q[1] * q[1] + q[2] * q[2]);
	m[0][1] = 2.0f * (q[0] * q[1] - q[2] * q[3]);
	m[0][2] = 2.0f * (q[2] * q[0] + q[1] * q[3]);
	m[0][3] = 0.0f;

	m[1][0] = 2.0f * (q[0] * q[1] + q[2] * q[3]);
	m[1][1] = 1.0f - 2.0f * (q[2] * q[2] + q[0] * q[0]);
	m[1][2] = 2.0f * (q[1] * q[2] - q[0] * q[3]);
	m[1][3] = 0.0f;

	m[2][0] = 2.0f * (q[2] * q[0] - q[1] * q[3]);
	m[2][1] = 2.0f * (q[1] * q[2] + q[0] * q[3]);
	m[2][2] = 1.0f - 2.0f * (q[1] * q[1] + q[0] * q[0]);
	m[2][3] = 0.0f;

	m[3][0] = 0.0f;
	m[3][1] = 0.0f;
	m[3][2] = 0.0f;
	m[3][3] = 1.0f;
}

void InitializeWindow(int argc, char* argv[])
{
	// initialize glut settings
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);
	glutInitWindowSize(1000 / 2, 1000 / 2);

	glutInitWindowPosition(0, 0);

	dispWindowIndex = glutCreateWindow("3D Model");

	trackball(quat, 90.0, 0.0, 0.0, 0.0);

	glutIdleFunc(idle);
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutSpecialFunc(special);
	glutMotionFunc(motion);
	glutMouseFunc(mouse);
	glutCloseFunc(close);
	//GLuint image = load   ("./my_texture.bmp");
	
	//glBindTexture(1,)

	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);

	// bind textures
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glEnable(GL_DEPTH_TEST);

	reshape(1000, 1000);

	/*glGenTextures(1, &dispBindIndex);
	glBindTexture(GL_TEXTURE_2D, dispBindIndex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);*/
}


void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60, 1, 0.1, 200);		
	glTranslatef(t[0], t[1], t[2] - 1.0f);
	glScalef(1, 1, 1);	
	GLfloat m[4][4],m1[4][4];
	build_rotmatrix(m, quat);
	gluLookAt(0, 0.2, 2.0, 0, 0, 0, 0, 1.0, 0);

	GLfloat r, g, b;
	glMultMatrixf(&m[0][0]);


	/////////////////////////////////
	//glEnable(GL_LIGHTING);
	//glEnable(GL_LIGHT0);
	//GLfloat diffuse0[4] = { 1.0, 1.0, 1.0, 1.0 };
	//GLfloat ambient0[4] = { 0.1, 0.1, 0.1, 1.0 };
	//GLfloat specular0[4] = { 1.0, 1.0, 1.0, 1.0 };
	//GLfloat light0_pos[4] = { 2.0, 2.0, 2.0, 1.0 };
	////GLfloat light0_pos[4] = { 0.3, 0.3, 0.5, 1.0 };
	//GLfloat spot_dir[3] = { -2.0f, 0.0f, -1.0f };


	//floor
	glTexImage2D(GL_TEXTURE_2D, 0, 3, 276, 276, 0, GL_RGB, GL_UNSIGNED_BYTE, mytexels);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		
	glEnable(GL_TEXTURE_2D);
	glBegin(GL_QUADS);
	for (float fl = -1.0; fl < 1.0; fl = fl + 0.2)
	{
		for (float fr = -1.0; fr < 1.0; fr = fr + 0.2)
		{
			glTexCoord2d(0.0, 0.0);
			glVertex3f(-0.2 + fl, 0.0, -0.2 + fr);
			glTexCoord2d(1.0, 0.0);
			glVertex3f(-0.2 + fl, 0.0, 0.2 + fr);
			glTexCoord2d(1.0, 1.0);
			glVertex3f(0.2 + fl, 0.0, 0.2 + fr);
			glTexCoord2d(0.0, 1.0);
			glVertex3f(0.2 + fl, 0.0, -0.2 + fr);
		}

	}
	glEnd();


	//wall
	glTexImage2D(GL_TEXTURE_2D, 0, 3, 512, 512, 0, GL_RGB, GL_UNSIGNED_BYTE, wall);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexEnvf(GL_TEXTURE_2D, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	glEnable(GL_TEXTURE_2D);
	glBegin(GL_QUADS);
	for (float fl = -1.0; fl < 1.0; fl = fl + 0.2)
	{
		for (float fr = -1.0; fr < 1.0; fr = fr + 0.2)
		{
			glTexCoord2d(0.0, 0.0);
			glVertex3f(-0.2, 0.0, 0.2);
			glTexCoord2d(1.0, 0.0);
			glVertex3f(-0.2, 0.4, 0.2);
			glTexCoord2d(1.0, 1.0);
			glVertex3f(0.2, 0.4, 0.2);
			glTexCoord2d(0.0, 1.0);
			glVertex3f(0.2, 0.0, 0.2);

			glTexCoord2d(1.0, 1.0);
			glVertex3f(-0.2, 0.4, -0.2);			
			glTexCoord2d(1.0, 0.0);
			glVertex3f(-0.2, 0.4, 0.2);			
			glTexCoord2d(0.0, 0.0);
			glVertex3f(-0.2, 0.0, 0.2);
			glTexCoord2d(0.0, 1.0);
			glVertex3f(-0.2, 0.0, -0.2);

			glTexCoord2d(0.0, 1.0);
			glVertex3f(0.2, 0.0, -0.2);
			glTexCoord2d(0.0, 0.0);
			glVertex3f(0.2, 0.0, 0.2);
			glTexCoord2d(1.0, 0.0);
			glVertex3f(0.2, 0.4, 0.2);
			glTexCoord2d(1.0, 1.0);
			glVertex3f(0.2, 0.4, -0.2);

		}

	}
	glEnd();

	/////////model loading�ϴ� �κ�/////////////
	//glEnable(GL_TEXTURE_2D);
	GLfloat x, y, z, nx, ny, nz;
	int v_id, vt_id, vn_id;
	int nFaces = models.objs[0].f.size();
	for (int k = 0; k < nFaces; k++) {
		int nPoints = models.objs[0].f[k].v_pairs.size(); //����Ʈ�� ���� üũ
		glBegin(GL_POLYGON);
		for (int i = 0; i < nPoints; i++) {
			v_id = models.objs[0].f[k].v_pairs[i].d[0];
			vt_id = models.objs[0].f[k].v_pairs[i].d[1];
			vn_id = models.objs[0].f[k].v_pairs[i].d[2];
			x = models.objs[0].v[v_id - 1].d[0];
			y = models.objs[0].v[v_id - 1].d[1];
			z = models.objs[0].v[v_id - 1].d[2];

			nx = models.objs[0].vn[vn_id - 1].d[0];
			ny = models.objs[0].vn[vn_id - 1].d[1];
			nz = models.objs[0].vn[vn_id - 1].d[2];
			glNormal3f(nx, ny, nz);
			glVertex3f(x, y, z);
		}
		glEnd();
	}

	glFlush();


	/*
	glEnable(GL_TEXTURE_2D);
	glBegin(GL_QUADS);
	glTexCoord2d(0.0, 0.0);
	glVertex3f(-0.2, -0.2, -0.2);
	glTexCoord2d(1.0, 0.0);
	glVertex3f(-0.2, 0.2, -0.2);
	glTexCoord2d(1.0, 1.0);
	glVertex3f(0.2, 0.2, -0.2);
	glTexCoord2d(0.0, 1.0);
	glVertex3f(0.2, -0.2, -0.2);

	glTexCoord2d(0.0, 0.0);
	glVertex3f(-0.2, -0.2, 0.2);
	glTexCoord2d(1.0, 0.0);
	glVertex3f(-0.2, 0.2, 0.2);
	glTexCoord2d(1.0, 1.0);
	glVertex3f(0.2, 0.2, 0.2);
	glTexCoord2d(0.0, 1.0);
	glVertex3f(0.2, -0.2, 0.2);

	glTexCoord2d(0.0, 0.0);
	glVertex3f(-0.2, -0.2, -0.2);
	glTexCoord2d(1.0, 0.0);
	glVertex3f(-0.2, -0.2, 0.2); 	
	glTexCoord2d(1.0, 1.0);
	glVertex3f(-0.2, 0.2, 0.2);
	glTexCoord2d(0.0, 1.0);
	glVertex3f(-0.2, 0.2, -0.2);

	glTexCoord2d(0.0, 0.0);
	glVertex3f(0.2, -0.2, -0.2);
	glTexCoord2d(1.0, 0.0);
	glVertex3f(0.2, -0.2, 0.2);
	glTexCoord2d(1.0, 1.0);
	glVertex3f(0.2, 0.2, 0.2);
	glTexCoord2d(0.0, 1.0);
	glVertex3f(0.2, 0.2, -0.2);

	glTexCoord2d(0.0, 0.0);
	glVertex3f(-0.2, -0.2, -0.2);
	glTexCoord2d(1.0, 0.0);
	glVertex3f(-0.2, -0.2, 0.2);
	glTexCoord2d(1.0, 1.0);
	glVertex3f(0.2, -0.2, 0.2);
	glTexCoord2d(0.0, 1.0);
	glVertex3f(0.2, -0.2, -0.2);

	glTexCoord2d(0.0, 0.0);
	glVertex3f(-0.2, 0.2, -0.2);
	glTexCoord2d(1.0, 0.0);
	glVertex3f(-0.2, 0.2, 0.2);
	glTexCoord2d(1.0, 1.0);
	glVertex3f(0.2, 0.2, 0.2);
	glTexCoord2d(0.0, 1.0);
	glVertex3f(0.2, 0.2, -0.2);
	*/
	
	
	glutSwapBuffers();
}



int main(int argc, char* argv[])
{
	ifstream fin("chair.obj");
	//cout << fin.is_open();
	models.loadObj(fin);
	fin.close();


	vertex = new Vertex[100000];
	vertex_color = new Vertex[100000];
	//mymesh = new Meshmodel[100000];
	mymesh = new MMesh[100000];

	int i,j,k=0;
	FILE* f = fopen("carpet.bmp", "rb");
	unsigned char info[54];
	fread(info, sizeof(unsigned char), 54, f); // read the 54-byte header
	int width = *(int*)&info[18];
	int height = *(int*)&info[22];
	int size = 3 * width * height;
	unsigned char* data = new unsigned char[size]; // allocate 3 bytes per pixel
	fread(data, sizeof(unsigned char), size, f); // read the rest of the data at once
	fclose(f);
	for (i = 0; i < width; i ++)
		for (j = 0; j < height; j ++)
		{
			mytexels[j][i][0] = data[k * 3 + 2];
			mytexels[j][i][1] = data[k * 3 + 1];
			mytexels[j][i][2] = data[k * 3];
			k++;
		}

	k = 0;
	FILE* f2 = fopen("mymap.bmp", "rb");
	unsigned char info2[54];
	fread(info2, sizeof(unsigned char), 54, f2); // read the 54-byte header
	int width2 = *(int*)&info2[18];
	int height2 = *(int*)&info2[22];
	int size2 = 3 * width2 * height2;
	unsigned char* data2 = new unsigned char[size2]; // allocate 3 bytes per pixel
	fread(data2, sizeof(unsigned char), size2, f2); // read the rest of the data at once
	fclose(f2);
	for (i = 0; i < width2; i++)
		for (j = 0; j < height2; j++)
		{
			wall[j][i][0] = data2[k * 3 + 2];
			wall[j][i][1] = data2[k * 3 + 1];
			wall[j][i][2] = data2[k * 3];
			k++;
		}
	/*
	FILE* fp;
	fp = fopen("Kitty.obj", "rb");
	//if (fp == NULL) {
	//	cout<<0;
	//}
	//else {
	//	cout << 1;
	//}
	int count = 0;
	int num = 0;
	char ch;
	float x, y, z;

	for (j = 0; j < 100000; j = j + 1)
	{
		count = fscanf(fp, "v %f %f %f /n", &x, &y, &z); //�����ϸ� �о�� ���� ������ ��ȯ, �����ϸ� EOF(-1)�� ��ȯ
		//error: ����ؼ� count���� 0�� ��ȯ��, ���� �������� �ϴµ�, v�� �����ϴ� �� �ϳ��� �ν� ���� 
		cout << count;
		if (count == 3)
		{
			vertex[j].X = x / scale;
			vertex[j].Y = y / scale;
			vertex[j].Z = z / scale;
			if (vertex[j].Z < zmin)
				zmin = vertex[j].Z;
			if (vertex[j].Z > zmax)
				zmax = vertex[j].Z;
		}
		else
			break;
	}

	for (j = 0; j < 100000; j = j + 1)
	{
		count = fscanf(fp, "vt %f %f %f /n", &x, &y, &z);
		if (count == 3)
		{
			vertex_color[j].X = x;
			vertex_color[j].Y = y;
			vertex_color[j].Z = z;
		} //uv�� ����
		else
			break;
	}
	fclose(fp);

	FILE* fpp;
	fpp = fopen("Kitty.obj", "r");
	float x1, y1, z1, x2, y2, z2, x3, y3, z3, x4, y4, z4;

	for (j = 0; j < 100000; j = j + 1)
	{
		count = fscanf(fp, "f %f/%f/%f %f/%f/%f %f/%f/%f %f/%f/%f /n", &x1, &y1, &z1, &x2, &y2, &z2, &x3, &y3, &z3, &x4, &y4, &z4);
		if (count == 12)
		{
			mymesh[j].V1 = x1;
			mymesh[j].V2 = x2;
			mymesh[j].V3 = x3;
			mymesh[j].V4 = x4;
			mymesh[j].T1 = y1;
			mymesh[j].T2 = y2;
			mymesh[j].T3 = y3;
			mymesh[j].T4 = y4;
		} //���°�� uv���� vertex�� �ش��ϴ°�, y: uv��
		else
			break;
	}
	fclose(fpp);
	*/
	InitializeWindow(argc, argv);

	display();

	glutMainLoop();
	delete[] vertex;
	delete[] vertex_color;
	return 0;
}