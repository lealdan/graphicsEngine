
/*** 2 x 2 Matrices ***/

/* Pretty-prints the given matrix, with one line of text per row of matrix. */
void mat22Print(GLdouble m[2][2]) {
	printf("\n|%f %f|\n|%f %f|\n\n", m[0][0], m[0][1], m[1][0], m[1][1]);
}

/* Pretty-prints the given matrix, with one line of text per row of matrix. */
void mat33Print(GLdouble m[3][3]) {
	printf("\n|%f %f %f|\n|%f %f %f|\n|%f %f %f|\n\n",
		m[0][0], m[0][1], m[0][2], m[1][0], m[1][1], m[1][2], m[2][0], m[2][1], m[2][2]);
}

/* Returns the determinant of the matrix m. If the determinant is 0.0, then the
matrix is not invertible, and mInv is untouched. If the determinant is not 0.0,
then the matrix is invertible, and its inverse is placed into mInv. */
GLdouble mat22Invert(GLdouble m[2][2], GLdouble mInv[2][2]) {
	GLdouble determinant = m[0][0] * m[1][1] - m[1][0] * m[0][1];

	if (determinant == 0.0) {
		return determinant;
	}
	mInv[0][0] = (1/determinant) * m[1][1];
	mInv[0][1] = (1/determinant) * -m[0][1];
	mInv[1][0] = (1/determinant) * -m[1][0];
	mInv[1][1] = (1/determinant) * m[0][0];

	return determinant;
}

/* Multiplies a 2x2 matrix m by a 2-column v, storing the result in mTimesV.
The output should not */
void mat221Multiply(GLdouble m[2][2], GLdouble v[2], GLdouble mTimesV[2]) {
	mTimesV[0] = m[0][0] * v[0] + m[0][1] * v[1];
	mTimesV[1] = m[1][0] * v[0] + m[1][1] * v[1];
}

/* Fills the matrix m from its two columns. */
void mat22Columns(GLdouble col0[2], GLdouble col1[2], GLdouble m[2][2]) {
	m[0][0] = col0[0];
	m[0][1] = col1[0];
	m[1][0] = col0[1];
	m[1][1] = col1[1];
}

/* Fills in a matrix with the identity matrix */
void mat33Identity(GLdouble m[3][3]) {
	m[0][0] = 1.0;
	m[0][1] = 0.0;
	m[0][2] = 0.0;
	m[1][0] = 0.0;
	m[1][1] = 1.0;
	m[1][2] = 0.0;
	m[2][0] = 0.0;
	m[2][1] = 0.0;
	m[2][2] = 1.0;
}

/* Multiplies the 3x3 matrix m by the 3x3 matrix n. */
void mat333Multiply(GLdouble m[3][3], GLdouble n[3][3], GLdouble mTimesN[3][3]) {
	mTimesN[0][0] = m[0][0] * n[0][0] + m[0][1] * n[1][0] + m[0][2] * n[2][0];
	mTimesN[0][1] = m[0][0] * n[0][1] + m[0][1] * n[1][1] + m[0][2] * n[2][1];
	mTimesN[0][2] = m[0][0] * n[0][2] + m[0][1] * n[1][2] + m[0][2] * n[2][2];
	mTimesN[1][0] = m[1][0] * n[0][0] + m[1][1] * n[1][0] + m[1][2] * n[2][0];
	mTimesN[1][1] = m[1][0] * n[0][1] + m[1][1] * n[1][1] + m[1][2] * n[2][1];
	mTimesN[1][2] = m[1][0] * n[0][2] + m[1][1] * n[1][2] + m[1][2] * n[2][2];
	mTimesN[2][0] = m[2][0] * n[0][0] + m[2][1] * n[1][0] + m[2][2] * n[2][0];
	mTimesN[2][1] = m[2][0] * n[0][1] + m[2][1] * n[1][1] + m[2][2] * n[2][1];
	mTimesN[2][2] = m[2][0] * n[0][2] + m[2][1] * n[1][2] + m[2][2] * n[2][2];
}

/* Multiplies the 3x3 matrix m by the 3x1 vector v. */
void mat331Multiply(GLdouble m[3][3], GLdouble v[3], GLdouble mTimesV[3]) {
	mTimesV[0] = m[0][0] * v[0] + m[0][1] * v[1] + m[0][2] * v[2];
	mTimesV[1] = m[1][0] * v[0] + m[1][1] * v[1] + m[1][2] * v[2];
	mTimesV[2] = m[2][0] * v[0] + m[2][1] * v[1] + m[2][2] * v[2];
}

/* Builds a 3x3 matrix representing 2D rotation and translation in homogeneous
coordinates. More precisely, the transformation first rotates through the angle
theta (in radians, counterclockwise), and then translates by the vector (x, y).
*/
void mat33Isometry(GLdouble theta, GLdouble x, GLdouble y, GLdouble isom[3][3]) {
	GLdouble rotation[3][3] = {{cos(theta), -sin(theta), 0.0}, {sin(theta), cos(theta), 0.0}, {0.0, 0.0, 1.0}};
	GLdouble translation[3][3] = {{1.0, 0.0, x}, {0.0, 1.0, y}, {0.0, 0.0, 1.0}};
	mat333Multiply(translation, rotation, isom);
}

/* Fills the matrix m from its two columns. */
void mat33Columns(GLdouble col0[3], GLdouble col1[3], GLdouble col2[3], GLdouble m[3][3]) {
	m[0][0] = col0[0];
	m[0][1] = col1[0];
	m[0][2] = col2[0];
	m[1][0] = col0[1];
	m[1][1] = col1[1];
	m[1][2] = col2[1];
	m[2][0] = col0[2];
	m[2][1] = col1[2];
	m[2][2] = col2[2];
}
/* Returns the transpose of 3x3 matrix, 'm', and places it in 'transpose' */
void mat33Transpose(GLdouble m[3][3], GLdouble transpose[3][3]) {
	transpose[0][0] = m[0][0];
	transpose[0][1] = m[1][0];
	transpose[0][2] = m[2][0];
	transpose[1][0] = m[0][1];
	transpose[1][1] = m[1][1];
	transpose[1][2] = m[2][1];
	transpose[2][0] = m[0][2];
	transpose[2][1] = m[1][2];
	transpose[2][2] = m[2][2];
}

/* Adds two 3x3 matrices together and places them in mPlusN */
void mat33Add(GLdouble m[3][3], GLdouble n[3][3], GLdouble mPlusN[3][3]) {
	mPlusN[0][0] = m[0][0] + n[0][0];
	mPlusN[0][1] = m[0][1] + n[0][1];
	mPlusN[0][2] = m[0][2] + n[0][2];
	mPlusN[1][0] = m[1][0] + n[1][0];
	mPlusN[1][1] = m[1][1] + n[1][1];
	mPlusN[1][2] = m[1][2] + n[1][2];
	mPlusN[2][0] = m[2][0] + n[2][0];
	mPlusN[2][1] = m[2][1] + n[2][1];
	mPlusN[2][2] = m[2][2] + n[2][2];
}

/* Scales a 3x3 matrix by a GLdouble 'c' and places the resulting matrix in mTimesC */
void mat33Scale(GLdouble m[3][3], GLdouble c, GLdouble mTimesC[3][3]) {
	mTimesC[0][0] = m[0][0] * c;
	mTimesC[0][1] = m[0][1] * c;
	mTimesC[0][2] = m[0][2] * c;
	mTimesC[1][0] = m[1][0] * c;
	mTimesC[1][1] = m[1][1] * c;
	mTimesC[1][2] = m[1][2] * c;
	mTimesC[2][0] = m[2][0] * c;
	mTimesC[2][1] = m[2][1] * c;
	mTimesC[2][2] = m[2][2] * c;
}

/* Given a length-1 3D vector axis and an angle theta (in radians), builds the
rotation matrix for the rotation about that axis through that angle. Based on
Rodrigues' rotation formula R = I + (sin theta) U + (1 - cos theta) U^2. */
void mat33AngleAxisRotation(GLdouble theta, GLdouble axis[3], GLdouble rot[3][3]) {
	GLdouble identity[3][3] = {{1.0, 0.0, 0.0}, {0.0, 1.0, 0.0}, {0.0, 0.0, 1.0}};
	GLdouble u[3][3] = {{0.0, -axis[2], axis[1]}, {axis[2], 0, -axis[0]}, {-axis[1], axis[0], 0}};
	GLdouble uSquare[3][3];
	GLdouble scaledU[3][3];
	GLdouble scaledUSquare[3][3];
	GLdouble scaledUPlusUSquare[3][3];

	mat333Multiply(u, u, uSquare);
	mat33Scale(u, sin(theta), scaledU);
	mat33Scale(uSquare, (1.0 - cos(theta)), scaledUSquare);
	mat33Add(scaledU, scaledUSquare, scaledUPlusUSquare);
	mat33Add(identity, scaledUPlusUSquare, rot);
}

/* Given two length-1 3D vectors u, v that are perpendicular to each other.
Given two length-1 3D vectors a, b that are perpendicular to each other. Builds
the rotation matrix that rotates u to a and v to b. */
void mat33BasisRotation(GLdouble u[3], GLdouble v[3], GLdouble a[3], GLdouble b[3],
        GLdouble rot[3][3]) {
	GLdouble r[3][3];
	GLdouble s[3][3];
	GLdouble uCrossV[3];
	GLdouble aCrossB[3];
	GLdouble rTranspose[3][3];

	vec3Cross(u, v, uCrossV);
	vec3Cross(a, b, aCrossB);

	mat33Columns(u, v, uCrossV, r);
	mat33Columns(a, b, aCrossB, s);

	mat33Transpose(r, rTranspose);
	mat333Multiply(s, rTranspose, rot);
}

void mat44Identity(GLdouble m[4][4]) {
	m[0][0] = 1.0;
	m[0][1] = 0.0;
	m[0][2] = 0.0;
	m[0][3] = 0.0;
	m[1][0] = 0.0;
	m[1][1] = 1.0;
	m[1][2] = 0.0;
	m[1][3] = 0.0;
	m[2][0] = 0.0;
	m[2][1] = 0.0;
	m[2][2] = 1.0;
	m[2][3] = 0.0;
	m[3][0] = 0.0;
	m[3][1] = 0.0;
	m[3][2] = 0.0;
	m[3][3] = 1.0;
}

/* Multiplies m by n, placing the answer in mTimesN. */
void mat444Multiply(GLdouble m[4][4], GLdouble n[4][4], GLdouble mTimesN[4][4]) {
	mTimesN[0][0] = m[0][0] * n[0][0] + m[0][1] * n[1][0] + m[0][2] * n[2][0] + m[0][3] * n[3][0];
	mTimesN[0][1] = m[0][0] * n[0][1] + m[0][1] * n[1][1] + m[0][2] * n[2][1] + m[0][3] * n[3][1];
	mTimesN[0][2] = m[0][0] * n[0][2] + m[0][1] * n[1][2] + m[0][2] * n[2][2] + m[0][3] * n[3][2];
	mTimesN[0][3] = m[0][0] * n[0][3] + m[0][1] * n[1][3] + m[0][2] * n[2][3] + m[0][3] * n[3][3];
	mTimesN[1][0] = m[1][0] * n[0][0] + m[1][1] * n[1][0] + m[1][2] * n[2][0] + m[1][3] * n[3][0];
	mTimesN[1][1] = m[1][0] * n[0][1] + m[1][1] * n[1][1] + m[1][2] * n[2][1] + m[1][3] * n[3][1];
	mTimesN[1][2] = m[1][0] * n[0][2] + m[1][1] * n[1][2] + m[1][2] * n[2][2] + m[1][3] * n[3][2];
	mTimesN[1][3] = m[1][0] * n[0][3] + m[1][1] * n[1][3] + m[1][2] * n[2][3] + m[1][3] * n[3][3];
	mTimesN[2][0] = m[2][0] * n[0][0] + m[2][1] * n[1][0] + m[2][2] * n[2][0] + m[2][3] * n[3][0];
	mTimesN[2][1] = m[2][0] * n[0][1] + m[2][1] * n[1][1] + m[2][2] * n[2][1] + m[2][3] * n[3][1];
	mTimesN[2][2] = m[2][0] * n[0][2] + m[2][1] * n[1][2] + m[2][2] * n[2][2] + m[2][3] * n[3][2];
	mTimesN[2][3] = m[2][0] * n[0][3] + m[2][1] * n[1][3] + m[2][2] * n[2][3] + m[2][3] * n[3][3];
	mTimesN[3][0] = m[3][0] * n[0][0] + m[3][1] * n[1][0] + m[3][2] * n[2][0] + m[3][3] * n[3][0];
	mTimesN[3][1] = m[3][0] * n[0][1] + m[3][1] * n[1][1] + m[3][2] * n[2][1] + m[3][3] * n[3][1];
	mTimesN[3][2] = m[3][0] * n[0][2] + m[3][1] * n[1][2] + m[3][2] * n[2][2] + m[3][3] * n[3][2];
	mTimesN[3][3] = m[3][0] * n[0][3] + m[3][1] * n[1][3] + m[3][2] * n[2][3] + m[3][3] * n[3][3];
}

/* Multiplies m by v, placing the answer in mTimesV. */
void mat441Multiply(GLdouble m[4][4], GLdouble v[4], GLdouble mTimesV[4]) {
	mTimesV[0] = m[0][0] * v[0] + m[0][1] * v[1] + m[0][2] * v[2] + m[0][3] * v[3];
	mTimesV[1] = m[1][0] * v[0] + m[1][1] * v[1] + m[1][2] * v[2] + m[1][3] * v[3];
	mTimesV[2] = m[2][0] * v[0] + m[2][1] * v[1] + m[2][2] * v[2] + m[2][3] * v[3];
	mTimesV[3] = m[3][0] * v[0] + m[3][1] * v[1] + m[3][2] * v[2] + m[3][3] * v[3];
}

/* Given a rotation and a translation, forms the 4x4 homogeneous matrix
representing the rotation followed in time by the translation. */
void mat44Isometry(GLdouble rot[3][3], GLdouble trans[3], GLdouble isom[4][4]) {
	GLdouble rotation[4][4] = {{rot[0][0], rot[0][1], rot[0][2], 0.0},
							 {rot[1][0], rot[1][1], rot[1][2], 0.0},
							 {rot[2][0], rot[2][1], rot[2][2], 0.0},
						     {0.0, 0.0, 0.0, 1.0}};


	GLdouble translation[4][4] = {{1.0, 0.0, 0.0, trans[0]},
	 							{0.0, 1.0, 0.0, trans[1]},
								{0.0, 0.0, 1.0, trans[2]},
								{0.0, 0.0, 0.0, 1.0}};

	mat444Multiply(translation, rotation, isom);
}

/* Given a rotation and translation, forms the 4x4 homogeneous matrix
representing the inverse translation followed in time by the inverse rotation.
That is, the isom produced by this function is the inverse to the isom
produced by mat44Isometry on the same inputs. */
void mat44InverseIsometry(GLdouble rot[3][3], GLdouble trans[3],
        GLdouble isom[4][4]) {
	GLdouble inverseRot33[3][3];
	GLdouble inverseTrans[4][4] = {{1.0, 0.0, 0.0, -trans[0]},
	 							{0.0, 1.0, 0.0, -trans[1]},
								{0.0, 0.0, 1.0, -trans[2]},
								{0.0, 0.0, 0.0, 1.0}};

	mat33Transpose(rot, inverseRot33);

	GLdouble inverseRot44[4][4] = {{inverseRot33[0][0], inverseRot33[0][1], inverseRot33[0][2], 0.0},
							 {inverseRot33[1][0], inverseRot33[1][1], inverseRot33[1][2], 0.0},
							 {inverseRot33[2][0], inverseRot33[2][1], inverseRot33[2][2], 0.0},
						     {0.0, 0.0, 0.0, 1.0}};

	mat444Multiply(inverseRot44, inverseTrans, isom);
}

/* Builds a 4x4 matrix representing orthogonal projection with a boxy viewing
volume [left, right] x [bottom, top] x [far, near]. That is, on the near plane
the box is the rectangle R = [left, right] x [bottom, top], and on the far
plane the box is the same rectangle R. Keep in mind that 0 > near > far. Maps
the viewing volume to [-1, 1] x [-1, 1] x [-1, 1]. */
void mat44Orthographic(GLdouble left, GLdouble right, GLdouble bottom, GLdouble top,
        GLdouble far, GLdouble near, GLdouble proj[4][4]) {
	proj[0][0] = 2.0 / (right - left);
	proj[0][1] = 0.0;
	proj[0][2] = 0.0;
	proj[0][3] = (-right - left)/(right - left);
	proj[1][0] = 0.0;
	proj[1][1] = 2.0 / (top - bottom);
	proj[1][2] = 0.0;
	proj[1][3] = (-top - bottom)/(top - bottom);
	proj[2][0] = 0.0;
	proj[2][1] = 0.0;
	proj[2][2] = 2.0 / (near - far);
	proj[2][3] = (-near - far)/(near - far);
	proj[3][0] = 0.0;
	proj[3][1] = 0.0;
	proj[3][2] = 0.0;
	proj[3][3] = 1.0;
}

/* Builds a 4x4 matrix that maps a projected viewing volume
[-1, 1] x [-1, 1] x [-1, 1] to screen [0, w - 1] x [0, h - 1] x [-1, 1]. */
void mat44Viewport(GLdouble width, GLdouble height, GLdouble view[4][4]) {
	view[0][0] = (width - 1.0) / 2.0;
	view[0][1] = 0.0;
	view[0][2] = 0.0;
	view[0][3] = (width - 1.0) / 2.0;
	view[1][0] = 0.0;
	view[1][1] = (height - 1.0) / 2.0;
	view[1][2] = 0.0;
	view[1][3] = (height - 1.0) / 2.0;
	view[2][0] = 0.0;
	view[2][1] = 0.0;
	view[2][2] = 1.0;
	view[2][3] = 0.0;
	view[3][0] = 0.0;
	view[3][1] = 0.0;
	view[3][2] = 0.0;
	view[3][3] = 1.0;
}

/* Builds a 4x4 matrix representing perspective projection. The viewing frustum
is contained between the near and far planes, with 0 > near > far. On the near
plane, the frustum is the rectangle R = [left, right] x [bottom, top]. On the
far plane, the frustum is the rectangle (far / near) * R. Maps the viewing
volume to [-1, 1] x [-1, 1] x [-1, 1]. */
void mat44Perspective(GLdouble lrbtfn[6], GLdouble proj[4][4]) {
	proj[0][0] = (-2.0 * lrbtfn[5]) / (lrbtfn[1] - lrbtfn[0]);
	proj[0][1] = 0.0;
	proj[0][2] = (lrbtfn[1] + lrbtfn[0]) / (lrbtfn[1] - lrbtfn[0]);
	proj[0][3] = 0.0;
	proj[1][0] = 0.0;
	proj[1][1] = (-2.0 * lrbtfn[5]) / (lrbtfn[3] - lrbtfn[2]);
	proj[1][2] = (lrbtfn[3] + lrbtfn[2]) / (lrbtfn[3] - lrbtfn[2]);
	proj[1][3] = 0.0;
	proj[2][0] = 0.0;
	proj[2][1] = 0.0;
	proj[2][2] = (-lrbtfn[5] - lrbtfn[4]) / (lrbtfn[5] - lrbtfn[4]);
	proj[2][3] = (2.0 * lrbtfn[5] * lrbtfn[4]) / (lrbtfn[5] - lrbtfn[4]);
	proj[3][0] = 0.0;
	proj[3][1] = 0.0;
	proj[3][2] = -1.0;
	proj[3][3] = 0.0;
}

/* We want to pass matrices into OpenGL, but there are two obstacles. First,
our matrix library uses double matrices, but OpenGL 2.x expects GLfloat
matrices. Second, C matrices are implicitly stored one-row-after-another, while
OpenGL expects matrices to be stored one-column-after-another. This function
plows through both of those obstacles. */
void mat44OpenGL(GLdouble m[4][4], GLfloat openGL[4][4]) {
	for (int i = 0; i < 4; i += 1)
		for (int j = 0; j < 4; j += 1)
			openGL[i][j] = m[j][i];
}
