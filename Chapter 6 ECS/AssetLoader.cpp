#include "AssetLoader.h"


#include <cstdio>
#include <cerrno>
#include <cstring>
#include <iostream>
#include <toml.h>
#include <SDL.h>

#define _CRT_SECURE_NO_WARNINGS
#define CGLTF_IMPLEMENTATION
#include "cgltf.h"


void LoadScene() {
	FILE* fp;
	char errbuf[200];

	fp = fopen("sample.toml", "r");
	if (!fp) {
		SDL_Log("cannot open sample.toml - %s", strerror(errno));
	}

	toml_table_t* conf = toml_parse_file(fp, errbuf, sizeof(errbuf));
	fclose(fp);

	if (!conf) {
		SDL_Log("cannot parse - %s", errbuf);
	}

	// 2. Traverse to a table.
	toml_table_t* server = toml_table_in(conf, "server");
	if (!server) {
		SDL_Log("missing [server]");
	}

	// 3. Extract values
	toml_datum_t host = toml_string_in(server, "host");
	if (!host.ok) {
		SDL_Log("cannot read server.host");
	}

	toml_array_t* portarray = toml_array_in(server, "port");
	if (!portarray) {
		SDL_Log("cannot read server.port");
	}

	printf("host: %s\n", host.u.s);
	printf("port: ");
	for (int i = 0; ; i++) {
		toml_datum_t port = toml_int_at(portarray, i);
		if (!port.ok) break;
		printf("%d ", (int)port.u.i);
	}
	printf("\n");

	// 4. Free memory
	free(host.u.s);
	toml_free(conf);
}

void LoadMesh() {
	cgltf_options options = { 0 };
	cgltf_data* data = NULL;
	cgltf_result result = cgltf_parse_file(&options, "scene.gltf", &data);
	if (result == cgltf_result_success)
	{
		/* TODO make awesome stuff */
		cgltf_free(data);
	}
	else
	{
		/* TODO error handling */
	}
}