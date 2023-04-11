/*
	GL OBJECT GENERATION CODE

//****VERTEX SETUP FOR FOLDED STAR *****
	//vertices will be passed in a vec4 packing style.
	//central point
	m_vertices.push_back(0.f);
	m_vertices.push_back(0.f);
	m_vertices.push_back(0.35f);
	m_vertices.push_back(1.f);

	//first index
	m_indices.push_back(0);

	auto sides = 8;
	auto d_sides = sides << 1;
	auto angle = glm::two_pi<float>()/d_sides;

	auto spokeLength = 0.9f;
	auto indentLength = spokeLength / 2.5;

	for (int i = 0; i <= d_sides; ++i) { //less than or equal so that it generates the last triangle

		//CAREFUL WITH LOGICAL OPERATIONS IN MULTIPLICATIONS!
		auto xCoord = glm::sin(i * angle) * ((i % 2 == 0) ? spokeLength : indentLength);
		m_vertices.push_back(xCoord);

		auto yCoord = glm::cos(i * angle) * ((i % 2 == 0) ? spokeLength : indentLength);
		m_vertices.push_back(yCoord);

		m_vertices.push_back(0.2f * (i%2==0));
		m_vertices.push_back(1.f);

		m_indices.push_back(i + 1);
	}
	m_vertices.shrink_to_fit();
	m_indices.shrink_to_fit();

	//VAO, VBO and IBO SETUP FOR VERTICES

	//Gl proper setup
	auto vertSize = GLsizei(4);
	auto elemNum = GLsizei(m_indices.size()); //could've also been m_vertices.size()/vertSize;

	glGenVertexArrays(1, &m_VAO);
	glGenBuffers(1, &m_VBO);

	glBindVertexArray(m_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(float), m_vertices.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(3, vertSize, GL_FLOAT, GL_FALSE, vertSize*sizeof(float), (void*)0); //vertex attrib pointer at location 3 (vertexInData)
	glEnableVertexArrayAttrib(m_VAO, 3);

	//glBindVertexArray(0); //CAN UNBIND VERTEX ARRAY
	//glBindBuffer(GL_ARRAY_BUFFER, 0); //CAN UNBIND BUFFER


	//******* TEXTURE VERTEX COORDINATES GENERATION ***********

	//central text coord (images are loaded from the bottom left, GL assumes TOP LEFT, but that's for later
	m_texCoords.push_back(.5f);
	m_texCoords.push_back(.5f);

	for (int i = 0; i <= d_sides; ++i) { //less than or equal so that it generates the last triangle

		//CAREFUL WITH LOGICAL OPERATIONS IN MULTIPLICATIONS!
		auto xCoord = glm::sin(i * angle) * ((i % 2 == 0) ? spokeLength : indentLength);
		m_texCoords.push_back(.5f + (.5f * xCoord));

		auto yCoord = glm::cos(i * angle) * ((i % 2 == 0) ? spokeLength : indentLength);
		m_texCoords.push_back(.5f + (.5f * yCoord));
	}
	m_texCoords.shrink_to_fit();


	//TEXTURE BUFFERS

	glGenBuffers(1, &m_tVBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_tVBO);
	glBufferData(GL_ARRAY_BUFFER, m_texCoords.size() * sizeof(float), m_texCoords.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(4, 2, GL_FLOAT, GL_FALSE, 2*sizeof(float), (void*)(0)); //vertex attrib pointer at location 4 (texInData)
	glEnableVertexArrayAttrib(m_VAO,4);

	auto img = IMG_Load("res/img/wall.jpg");
	glGenTextures(1, &m_TEX);
	//std::cout << "Wall Texture name: " << m_TEX << std::endl; //BUFFER NAME
	glActiveTexture(GL_TEXTURE1);
	//int active;
	//glGetIntegerv(GL_ACTIVE_TEXTURE, &active);
	//std::cout << "Currently active texture: " << active << std::endl; //ACTIVE TEXTURE NAME
	glBindTexture(GL_TEXTURE_2D, m_TEX);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//std::cout << SDL_GetPixelFormatName(img->format->format) << std::endl; //READS IMAGE FORMAT
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, img->w, img->h, 0, GL_RGB, GL_UNSIGNED_BYTE, img->pixels);
	glGenerateMipmap(GL_TEXTURE_2D); //has to be set
	SDL_FreeSurface(img); //CLEANUP

	glProgramUniform1i(m_programs[2], 8, 1); //set texture unit 1 to location 8 (sampler2D floorTexture)

	auto img2 = IMG_Load("res/img/lachesis.png");
	glGenTextures(1, &m_TEX2);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, m_TEX2);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img2->w, img2->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, img2->pixels);
	glGenerateMipmap(GL_TEXTURE_2D);
	SDL_FreeSurface(img2);

	glProgramUniform1i(m_programs[2], 9, 2); //set texture unit 2 to location 9 (sampler2D snakeTexture)

	
	//glEnable(GL_LINE_SMOOTH); //antialiasing
	//glLineWidth(3.f); //self-explanatory, but requires the previous statement

	//auto texName = "floorTexture";
	//std::cout << texName << " Uniform location : " << glGetUniformLocation(m_programs[2], texName) << std::endl;
	//texName = "snakeTexture";
	//std::cout << texName << " Uniform location : " << glGetUniformLocation(m_programs[2], texName) << std::endl;

	//glEnable(GL_CULL_FACE);
	//glFrontFace(GL_CW);

	//glEnable(GL_BLEND); allows colour blending, but may not be necessary depending on what is being done
	//glBlendFunc(GL_SRC_ALPHA, GL_DST_ALPHA);

//*/

/*
	//GL RENDER

	glClearColor(0.1f, 0.1f, 0.2f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  //DEPTH BUFFER NEEDED FOR, WELL, DEPTH

	//model, view, projection matrices
	auto model = glm::mat4(1.f); //identity matrix
	//rotates ~-1 radian around positive x-axis (top rotates away from reference)
	model = glm::rotate(model, -1.1f, glm::vec3(1.f, 0.f, 0.f)); 

	auto view = glm::mat4(1.f);
	//moves 2.5 units away from reference on the z axis (positive z points towards reference
	view = glm::translate(view, glm::vec3(0.f, 0.f, -2.5f));

	auto projection = glm::mat4(1.f);
	//perspective: FoV, Aspect Ratio, Near, Far
	projection = glm::perspective(1.f, 1.f, 0.1f, 100.f);

	//PROGRAM PIPELINE SETUP
	
	glBindProgramPipeline(m_pipeline);
	glUseProgramStages(m_pipeline, GL_VERTEX_SHADER_BIT, m_programs[0]);
	glUseProgramStages(m_pipeline, GL_FRAGMENT_SHADER_BIT, m_programs[2]);

	//uniform set for model-view-projection (vertex shader program)
	glProgramUniformMatrix4fv(m_programs[0], 0, 1, GL_FALSE, glm::value_ptr(projection));
	glProgramUniformMatrix4fv(m_programs[0], 1, 1, GL_FALSE, glm::value_ptr(view));
	glProgramUniformMatrix4fv(m_programs[0], 2, 1, GL_FALSE, glm::value_ptr(model));

	//refresher: program, location, value
	glProgramUniform1f(m_programs[2], 7, m_imgAngle); //specifics: shader01.frag program, location 7 (angle)

	//for reference: https://registry.khronos.org/OpenGL-Refpages/gl4/html/glProgramUniform.xhtml
	auto transfMat4 = glm::mat4(1.f);
	auto vec3z = glm::vec3(0.f, 0.f, 1.f);
	transfMat4 = glm::rotate(transfMat4, m_starAngle, vec3z);

	//std::cout << "Vertex matrix data:\n" << glm::to_string(transfMat4) << std::endl;

	//refresher: program, location, number of elements, should transpose, pointer to data
	glProgramUniformMatrix4fv(m_programs[0], 5, 1, GL_FALSE, glm::value_ptr(transfMat4)); //shader00.vert program, location 5 (vertex transformation matrix)

	auto imgMat = glm::mat4(1.f);
	auto imgOffset = glm::vec3(.5f, .5f, 0.f);
	imgMat = glm::translate(imgMat, imgOffset);

	/*********************************************************************************************
	 * NOTE:
	 * m_starAngle is used because they both refer to vertices. In Uniform location 3 we store star vertices,
	 *		but in location 4 we store (star-shaped) sampling coordinates. If the sampling points remain fixed
	 *		to the image they sample, when we rotate the drawing, the image will look rotated. The calls to
	 *		translate are also used because the sampling points are normalized to [0,1], meaning that
	 *		what we see as the center of the image is actually its (0.5, 0.5) point. We need to set the center
	 *		of rotation of the sampling to (0.5, 0.5), rotate the sampling points, and then return the origin
	 *		to its initial state
	 ********************************************************************************************//*/
	imgMat = glm::rotate(imgMat, m_starAngle, vec3z);

	imgMat = glm::translate(imgMat, -imgOffset);
	//std::cout << "Image matrix data:\n" << glm::to_string(imgMat) << std::endl;
	glProgramUniformMatrix4fv(m_programs[0], 6, 1, GL_FALSE, glm::value_ptr(imgMat)); //shader00.vert program, location 6 (image transformation matrix)

	glBindVertexArray(m_VAO);
	glEnableVertexArrayAttrib(m_VAO, 3);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDrawArrays(GL_TRIANGLE_FAN, 0, m_indices.size());

	transfMat4 = glm::translate(transfMat4, glm::vec3(0.f, 0.f, glm::sin(m_starAngle)));
	glProgramUniformMatrix4fv(m_programs[0], 5, 1, GL_FALSE, glm::value_ptr(transfMat4)); //shader00.vert program, location 5 (vertex transformation matrix)

	glUseProgramStages(m_pipeline, GL_FRAGMENT_SHADER_BIT, m_programs[1]);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	//glDrawArrays(GL_TRIANGLE_FAN, 0, m_indices.size());

	Geometry geom = m_gg.getGeometry("Hexahedron");
	glBindVertexArray(geom.vao);
	glDrawElements(GL_TRIANGLE_STRIP, geom.ibo_sz, GL_UNSIGNED_INT, 0);

	glBindVertexArray(0); //unbind array
*/