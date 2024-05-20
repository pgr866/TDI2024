#include <stdio.h>
#include <cmath>
#include <C_Matrix.hpp>
#include <C_Image.hpp>

/**
 * @brief Funcion recursiva que rota una imagen por un angulo especificado en sentido de las agujas del reloj.
 *
 * @param ptr_imagen Un puntero a un objeto de tipo C_Image que representa la imagen a rotar.
 * @param angulo Un valor de tipo double que especifica el angulo de rotacion en grados.
 */
void RotarImagen(C_Image* ptr_imagen, double angulo) {

	// Si el angulo es negativo, se le suma 360º hasta convertirlo en un angulo positivo.
	if (angulo < 0.0)
		RotarImagen(ptr_imagen, 360.0 + angulo);
	else {
		// Se rota 90º a la derecha hasta que el angulo sea menor que 90º
		while (angulo > 90.0) {
			RotarImagen(ptr_imagen, 90.0);
			angulo -= 90.0;
		}

		// Caso base de la recursividad
		// Se copia el contenido del puntero de la imagen original que llega por parametro
		C_Image original = *ptr_imagen;

		// Se guarda la cantidad de filas y columnas de la imagen original
		C_Matrix::IndexT old_rows = original.RowN();
		C_Matrix::IndexT old_cols = original.ColN();

		// Se calculan seno y coseno del angulo en radianes
		double sen_a = sin(angulo * M_PI / 180.0);
		double cos_a = cos(angulo * M_PI / 180.0);

		// Se guarda la cantidad de filas y columnas de la imagen rotada
		C_Matrix::IndexT rows = round(cos_a * old_rows + sen_a * old_cols);
		C_Matrix::IndexT cols = round(cos_a * old_cols + sen_a * old_rows);
		
		// Se crea el objeto de la imagen rotada resultante, indexada a 0 e inicializada en negro
		C_Image result(0, rows - 1, 0, cols - 1, 0);

		// Se declaran las variables usadas al iterar las cordenadas de la imagen original
		C_Matrix::IndexT old_i;
		C_Matrix::IndexT old_j;

		// Se recorre la imagen resultante para asignarle el pixel que le correspondiente de la imagen original
		for (C_Matrix::IndexT i = result.FirstRow(); i <= result.LastRow(); ++i) {
			for (C_Matrix::IndexT j = result.FirstCol(); j <= result.LastCol(); ++j) {
				// Se calcula a que coordenadas de la imagen original corresponde cada pixel de la imagen rotada
				old_i = round(sen_a * sen_a * old_rows + cos_a * i - sen_a * j);
				old_j = round(sen_a * i + cos_a * j - sen_a * cos_a * old_rows);
				// Se realiza la asignacion tras comprobar que las coordenadas calculadas estan en los limites de la imagen original
				if (old_i >= original.FirstRow() && old_i <= original.LastRow() &&  old_j >= original.FirstCol() && old_j <= original.LastCol())
					result(i, j) = original(old_i, old_j);
			}
		}

		// Se almacena el objeto de la imagen rotada en el puntero que llega por parametro
		*ptr_imagen = result;
		// Se libera la memoria de los objetos de imagen auxiliares
		original.Free();
		result.Free();
	}
}

/**
 * @brief Funcion principal del programa.
 *
 * Verifica que se proporcionen dos argumentos por linea de comandos:
 * el nombre de la imagen (acabado en .bmp) ubicada en la carpeta Run,
 * el nombre de la imagen rotada resultante (acabado en .bmp) ubicada en la carpeta Run y
 * el angulo de rotacion en grados.
 * 
 * Luego carga la imagen, la rota utilizando la funcion RotarImagen, y finalmente escribe la imagen resultante en un archivo en la carpeta Run.
 * Ejemplo de ejecucion en linea de comandos de Windows:
 * set "ejecutable=..\x64\Debug\TDI.exe"
 * %ejecutable% Aguadulce_Gris.bmp Aguadulce_Gris_60.bmp 60
 * 
 * Más ejemplos de ejecuciones automaticas en Run\Run.bat.
 * 
 * @param argc El numero de argumentos de la linea de comandos.
 * @param argv Un array de caracteres que contiene los argumentos de la linea de comandos.
 * @return Devuelve un entero que indica el estado de salida del programa. 0 indica ejecucion correcta, otros valores indican algun tipo de error.
 */
int main(int argc, char** argv) {

	// Se comprueba que se proporcionan tres argumentos por linea de comandos
	if (argc != 4) {
		printf("Error: Debe proporcionar los siguientes argumentos por linea de comandos:");
		printf("- Nombre de la imagen (acabado en .bmp)");
		printf("- Nombre de la imagen de salida rotada (acabado en .bmp)");
		printf("- Angulo a rotar en grados");
		return 1;
	}

	// Se cargan los argumentos
	char* nombre_imagen_entrada = argv[1];
	char* nombre_imagen_salida = argv[2];
	double angulo = std::stod(argv[3]);

	// Se carga la imagen de entrada en el objeto imagen, se reindexa a 0 y se pasa a tono de grises
	C_Image imagen;
	imagen.Read(nombre_imagen_entrada);
	imagen.Reindex(0, 0);
	imagen.Grey();

	// Se rota la imagen referenciada
	RotarImagen(&imagen, angulo);

	// Se escribe la imagen resultante
	imagen.Write(nombre_imagen_salida);
	// Se libera la memoria del objeto de imagen utilizado
	imagen.Free();
	return 0;
}