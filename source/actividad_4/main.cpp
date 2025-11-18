#include "raylib.h" 

void movimiento_x(Vector2& posicion);// Movimiento en el eje X
void salto(Vector2& posicion,int& direccion, bool& saltos,float altura_max);//Simulacion de salto	

int main() {

	int  altoPantalla = 720;
	int anchoPantalla = 1026;
	int direccion = -1;
	int indice_color = GetRandomValue(0, 9);//Indice inicial del color aleatorio
	
	float limite_salto = 150.0f; //Distancia maxima del salto
	
	bool saltando = false; //Estado del salto
	bool apagar = false; // Estado de los textos
	bool cambiar_color = false;// Estado del cambio de color
	
	Color color_fondo = RAYWHITE;// Color inicial del fondo
	Color color_aleatorio[10] = { DARKBROWN , SKYBLUE ,PURPLE, DARKBLUE , PINK , YELLOW, GREEN, ORANGE , MAROON, WHITE };
	
	InitWindow(anchoPantalla, altoPantalla, "Actividad 4 - Raylib");
	InitAudioDevice(); // Initialize audio device

	Texture2D personaje = LoadTexture("../sprite/lagarto.png");
	
	Sound efecto_salto = LoadSound("../sounds/salto.wav");// Carga del efecto de sonido
	Sound efecto_boton = LoadSound("../sounds/boton.wav");// Carga del efecto de sonido

	Vector2 posicion_inicial = { anchoPantalla / 3.0f, altoPantalla / 2.0f };// Posicion inicial del personaje
	Vector2 posicion_pj = { anchoPantalla / 3.0f, altoPantalla / 2.0f };// Posicion del personaje que se puede modificar
	Vector2 posicion_mouse;

	SetTextureFilter(personaje, TEXTURE_FILTER_BILINEAR);

	SetTargetFPS(60);

	while (!WindowShouldClose()) {

		int nuevo_indice = GetRandomValue(0, 9);// Nuevo indice para el color aleatorio
		
		movimiento_x(posicion_pj);
		
		posicion_mouse = GetMousePosition();
		
		float distancia_x = posicion_mouse.x - 950;//Distancia del mouse al centro del circulo en el eje X
		float distancia_y = posicion_mouse.y - 100;//Distancia del mouse al centro del circulo en el eje Y
		float distancia = ((distancia_x * distancia_x) + (distancia_y * distancia_y));// Calculo de la distancia al cuadrado

		if (IsKeyDown(KEY_SPACE) && !saltando) { // Comprueba que se presino el espacio y si esta en el suelo
			
			saltando = true;
			PlaySound(efecto_salto); // Reproduce el efecto de sonido del salto
		}

		salto(posicion_pj, direccion, saltando, limite_salto);// Llama a la funcion de salto

		if (IsKeyReleased(KEY_R)) { // Reinicia la posicion del personaje
			
			posicion_pj = posicion_inicial;
		}
		
		if (IsKeyPressed(KEY_M)) {//Hace que los textos desaparecan de la pantalla
			
			apagar = !apagar;
		}

		if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && distancia <= 30.0f * 30.0f) { // Comprueba que el cursor este dentro del circulo y a su vez que se haga click dentro de el circulo
			
			PlaySound(efecto_boton); // Reproduce el efecto de sonido del boton
			
			if (nuevo_indice == indice_color) {// Evita que se repita el mismo color
				
				nuevo_indice = (nuevo_indice + 1) % 10;
			}

			indice_color = nuevo_indice;
			color_fondo = color_aleatorio[indice_color]; // Cambia el color de fondo

		}

		BeginDrawing();
		ClearBackground(color_fondo);
		DrawTextureEx(personaje, posicion_pj, 0.0f, 0.25f, WHITE);

		if (!apagar){
		      
			DrawText("Usa las flechas izquierda y derecha para mover el personaje", 10, 10, 20, BLACK);
			DrawText("Presiona espacio para saltar y R para reiniciar la posicion", 10, 40, 20, BLACK);
			DrawText(TextFormat("Posicion : %.2f, %.2f", posicion_pj.x, posicion_pj.y), 10, 70, 20, BLACK);
			DrawText("Haz click izquierdo para cambiar el color de fondo", 10, 100, 20, BLACK);
		}

		DrawCircle(950, 100, 30.0f, RED);//Boton para cambiar color de fondo
		DrawRectangle(0, 470, anchoPantalla, altoPantalla - 600, LIGHTGRAY);// Suelo
		
		EndDrawing();

	}
	UnloadSound(efecto_salto);
	UnloadSound(efecto_boton);
	CloseAudioDevice();// Cierra el dispositivo de audio
	UnloadTexture(personaje);
	CloseWindow();


	return 0;
}

void movimiento_x(Vector2& posicion) {
	if (IsKeyDown(KEY_RIGHT)) {
		posicion.x += 2.1f;

	}
	if (IsKeyDown(KEY_LEFT)) {
		posicion.x -= 2.1f;
	}
}
void salto(Vector2& posicion,int& direccion,bool& saltos, float altura_max) {
	
	float suelo = 360.0f;

	if (saltos) { // Si se esta en estado de salto se le incrementa o decrementa la posicion en Y dependiendo de la direccion

		posicion.y += direccion * 5.0f;
	}
	if (posicion.y <= suelo - altura_max) {
		direccion = 1;
	}
	
	if (posicion.y >= suelo) { // Si el personaje llega al suelo se reinician las variables
		posicion.y = suelo;
		saltos = false;
		direccion = -1;
	}

}