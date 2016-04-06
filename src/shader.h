#include<GLES2/gl2.h>	// FLAG use better encapsulation for GL vars?  or does this keep it simple?

extern GLuint envprogram;
extern GLint envvertpointer;
extern GLint envmatrixpointer;
extern GLuint objprogram;
extern GLint objvertpointer;
extern GLint objmatrixpointer;
extern GLint objcolorpointer;
extern GLuint hudprogram;
extern GLint hudvertpointer;
extern GLint hudmatrixpointer;
extern GLint huduvpointer;
extern GLint hudtexturepointer;

extern void (*initShaderPtr)(void);
