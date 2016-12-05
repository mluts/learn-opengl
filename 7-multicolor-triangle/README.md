## Multicolor Triangle

Here i've implemented a VBO (Vertex Buffer Object) which contains the colors
along with vertices coordinates. What turned out is that if you specify
different colors for vertices in one triangle, color between vertices will be
interpolated by OpenGL, it results in triangle which looks like color palette in
paint.

Also, i specified a "color" variable location and specified that location in
`glVertexAttribPointer` and `glEnableVertexAttribArray` which is new to me.

Learned that there is a some kind of legacy for `glVertexAttribPointer` that
last argument should be of type `(GLvoid *)` and this is an offset from first
element in VBO.

Also, passing `inColor` variable from vertex shader to fragment shader here. It
is an interesting moment too. The primary thing not to forget, is to assign some
value to `inColor` in vertex shader.
