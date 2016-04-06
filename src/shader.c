
// Includes
//{{{
#include<GLES2/gl2.h>
#include<stdlib.h>
#include <stdio.h> // FLAG delete me!
//}}}

// Variables
//{{{
GLuint envprogram;
GLint envvertpointer;
GLint envmatrixpointer;
GLuint objprogram;
GLint objvertpointer;
GLint objmatrixpointer;
GLint objcolorpointer;
GLuint hudprogram;
GLint hudvertpointer;
GLint hudmatrixpointer;
GLint huduvpointer;
GLint hudtexturepointer;
//}}}


// Printlog functions
//{{{
static void printprogramlog(GLuint program)
{
	if(!glIsProgram)
	{
		//debug_errormessage("Invalid Program Passed.\n");
		return;
	}
	int maxlen;

	glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxlen);
	char* infolog = (char*)malloc(sizeof(char)*maxlen);
	glGetProgramInfoLog(program, maxlen, 0, infolog);
	//debug_errormessage(infolog);
	printf("%s",infolog);// FLAG
	free(infolog);
}

static void printshaderlog(GLuint shader)
{
	if(!glIsShader(shader))
	{
		//debug_errormessage("Invalid Shader Passed.\n");
		return;
	}

	int maxlen;

	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxlen);
	char* infolog = (char*)malloc(sizeof(char)*maxlen);
	glGetShaderInfoLog(shader, maxlen, 0, infolog);
	//debug_errormessage(infolog);
	printf("%s",infolog);// FLAG
	free(infolog);

}
//}}}

// Init
//{{{
static void initShader()
{

	// Environment Program
	{//{{{

		GLint check;

		envprogram = glCreateProgram();

		GLuint v = glCreateShader(GL_VERTEX_SHADER);
		const GLchar* vs[1];
		vs[0] = "#version 100\n\
			precision highp float;\n\
			\n\
			uniform mat4 testmatrix;\n\
			attribute vec3 vert;\n\
			varying vec3 xyz;\n\
			void main()\n\
			{\n\
				xyz = vec3(vert[0],vert[1],vert[2]);\n\
				gl_Position = testmatrix*vec4(vert[0], vert[1], vert[2], 1.0);\n\
			}";
		glShaderSource(v, 1, vs, 0);
		glCompileShader(v);
		check = GL_FALSE;
		glGetShaderiv(v, GL_COMPILE_STATUS, &check);
		if(check!=GL_TRUE)
		{
			//debug_errormessage("Vertex Shader compilation failure: ");
			printshaderlog(v);
			exit(1);
		}
		glAttachShader(envprogram, v);

		GLuint f = glCreateShader(GL_FRAGMENT_SHADER);
		const GLchar* fs[1];
		fs[0] = "#version 100\n\
			precision highp float;\n\
			\n\
			varying vec3 xyz;\n\
			void main()\n\
			{\n\
				float w = 0.1;\n\
				bool xb = mod(xyz[0]+w/2.0,1.0)<=w;\n\
				bool yb = mod(xyz[1]+w/2.0,1.0)<=w;\n\
				bool zb = mod(xyz[2]+w/2.0,1.0)<=w;\n\
				float c = float((xb&&yb)||(yb&&zb)||(xb&&zb));\n\
				vec3 c1 = vec3(0.1,0.1,0.1);\n\
				vec3 c2 = vec3(0.9, 0.9, 0.9);\n\
				c1 = mix(c1, c2, c);\n\
				gl_FragColor = vec4(c1, 1.0);\n\
			}";
		/* floating point suffix unsupported????
		fs[0] = "#version 100\n\
			precision highp float;\n\
			\n\
			varying vec2 xy;\n\
			void main()\n\
			{\n\
				float c = (floor(mod(xy[0],2.0f))+floor(mod(xy[1],2.0f)))/2.0f;\n\
				vec3 c1 = vec3(1.0f, 0.8f, 0.0f);\n\
				vec3 c2 = vec3(0.5f, 0.5f, 0.0f);\n\
				c1 = mix(c1, c2, c);\n\
				gl_FragColor = vec4(c1, 1.0);\n\
			}";
			*/
		glShaderSource(f, 1, fs, 0);
		glCompileShader(f);
		check = GL_FALSE;
		glGetShaderiv(f, GL_COMPILE_STATUS, &check);
		if(check!=GL_TRUE)
		{
			//debug_errormessage("Fragment Shader compilation failure: ");
			printshaderlog(f);
			exit(1);
		}
		glAttachShader(envprogram, f);

		glLinkProgram(envprogram);
		check = GL_TRUE;
		glGetProgramiv(envprogram, GL_LINK_STATUS, &check);
		if(check!=GL_TRUE)
		{
			//debug_errormessage("Program Link failure: ");
			printprogramlog(envprogram);
			exit(1);
		}

		// Set pointer to "vert" location in shader
		envvertpointer = glGetAttribLocation(envprogram, "vert");
		if(envvertpointer==-1)
		{
			//debug_errormessage("Invalid variable name.\n");
			exit(1);
		}
		envmatrixpointer = glGetUniformLocation(envprogram, "testmatrix");
		if(envmatrixpointer==-1)
		{
			//debug_errormessage("Invalid variable name.\n");
			exit(1);
		}

	}//}}}

	// Object Program
	{//{{{

		GLint check;

		objprogram = glCreateProgram();

		GLuint v = glCreateShader(GL_VERTEX_SHADER);
		const GLchar* vs[1];
		vs[0] = "#version 100\n\
			precision highp float;\n\
			\n\
			uniform mat4 testmatrix;\n\
			attribute vec3 vert;\n\
			varying float z;\n\
			void main()\n\
			{\n\
				z = (vert[2]+0.5)/3.0;\n\
				gl_Position = testmatrix*vec4(vert[0], vert[1], vert[2], 1.0);\n\
			}";
		glShaderSource(v, 1, vs, 0);
		glCompileShader(v);
		check = GL_FALSE;
		glGetShaderiv(v, GL_COMPILE_STATUS, &check);
		if(check!=GL_TRUE)
		{
			//debug_errormessage("Vertex Shader compilation failure: ");
			printshaderlog(v);
			exit(1);
		}
		glAttachShader(objprogram, v);

		GLuint f = glCreateShader(GL_FRAGMENT_SHADER);
		const GLchar* fs[1];
		fs[0] = "#version 100\n\
			precision highp float;\n\
			\n\
			uniform vec3 color;\n\
			varying float z;\n\
			void main()\n\
			{\n\
				gl_FragColor = vec4(z*color, 1.0);\n\
			}";
		glShaderSource(f, 1, fs, 0);
		glCompileShader(f);
		check = GL_FALSE;
		glGetShaderiv(f, GL_COMPILE_STATUS, &check);
		if(check!=GL_TRUE)
		{
			//debug_errormessage("Fragment Shader compilation failure: ");
			printshaderlog(f);
			exit(1);
		}
		glAttachShader(objprogram, f);

		glLinkProgram(objprogram);
		check = GL_TRUE;
		glGetProgramiv(objprogram, GL_LINK_STATUS, &check);
		if(check!=GL_TRUE)
		{
			//debug_errormessage("Program Link failure: ");
			printprogramlog(objprogram);
			exit(1);
		}

		// Set pointer to "vert" location in shader
		objvertpointer = glGetAttribLocation(objprogram, "vert");
		if(objvertpointer==-1)
		{
			//debug_errormessage("Invalid variable name.\n");
			exit(1);
		}
		objmatrixpointer = glGetUniformLocation(objprogram, "testmatrix");
		if(objmatrixpointer==-1)
		{
			//debug_errormessage("Invalid variable name.\n");
			exit(1);
		}
		objcolorpointer = glGetUniformLocation(objprogram, "color");
		if(objmatrixpointer==-1)
		{
			//debug_errormessage("Invalid variable name.\n");
			exit(1);
		}
	}//}}}

	// HUD (Text & Icon) Program
	{//{{{

		GLint check;

		hudprogram = glCreateProgram();

		GLuint v = glCreateShader(GL_VERTEX_SHADER);
		const GLchar* vs[1];
		vs[0] = "#version 100\n\
			precision highp float;\n\
			\n\
			uniform mat4 testmatrix;\n\
			attribute vec3 vert;\n\
			attribute vec2 uv;\n\
			varying vec2 ftex;\n\
			void main()\n\
			{\n\
				gl_Position = testmatrix*vec4(vert, 1.0);\n\
				ftex = uv;\n\
			}";
		glShaderSource(v, 1, vs, 0);
		glCompileShader(v);
		check = GL_FALSE;
		glGetShaderiv(v, GL_COMPILE_STATUS, &check);
		if(check!=GL_TRUE)
		{
			//debug_errormessage("Vertex Shader compilation failure: ");
			printshaderlog(v);
			exit(1);
		}
		glAttachShader(hudprogram, v);

		GLuint f = glCreateShader(GL_FRAGMENT_SHADER);
		const GLchar* fs[1];
		fs[0] = "#version 100\n\
			precision highp float;\n\
			\n\
			uniform sampler2D texturesampler;\n\
			varying vec2 ftex;\n\
			void main()\n\
			{\n\
				gl_FragColor = texture2D(texturesampler,ftex);\n\
			}";
		glShaderSource(f, 1, fs, 0);
		glCompileShader(f);
		check = GL_FALSE;
		glGetShaderiv(f, GL_COMPILE_STATUS, &check);
		if(check!=GL_TRUE)
		{
			//debug_errormessage("Fragment Shader compilation failure: ");
			printshaderlog(f);
			exit(1);
		}
		glAttachShader(hudprogram, f);

		glLinkProgram(hudprogram);
		check = GL_TRUE;
		glGetProgramiv(hudprogram, GL_LINK_STATUS, &check);
		if(check!=GL_TRUE)
		{
			//debug_errormessage("Program Link failure: ");
			printprogramlog(hudprogram);
			exit(1);
		}

		// Set pointer to "vert" location in shader
		hudvertpointer = glGetAttribLocation(hudprogram, "vert");
		if(hudvertpointer==-1)
		{
			//debug_errormessage("Invalid variable name.\n");
			exit(1);
		}
		huduvpointer = glGetAttribLocation(hudprogram, "uv");
		if(huduvpointer==-1)
		{
			//debug_errormessage("Invalid variable name uv.\n");
			exit(1);
		}
		hudmatrixpointer = glGetUniformLocation(hudprogram, "testmatrix");
		if(hudmatrixpointer==-1)
		{
			//debug_errormessage("Invalid variable name.\n");
			exit(1);
		}
		hudtexturepointer = glGetUniformLocation(hudprogram, "texturesampler");
		if(hudtexturepointer==-1)
		{
			//debug_errormessage("Invalid variable name.\n");
			exit(1);
		}
	}//}}}

}

void (*initShaderPtr)(void) = initShader;
//}}}

