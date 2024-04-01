#include "client.h"

int main(void)
{
	/*---------------------------------------------------PARTE 2-------------------------------------------------------------*/

	int socket_client;
	char* ip;
	char* puerto;
	char* configMessage;

	t_log* logger;
	t_config* config;

	/* ---------------- LOGGING ---------------- */

	logger = iniciar_logger();

	// Usando el logger creado previamente
	// Escribi: "Hola! Soy un log"

	log_info(logger, "Holaaaa, soy un log");

	/* ---------------- ARCHIVOS DE CONFIGURACION ---------------- */

	config = iniciar_config();


	// Usando el config creado previamente, leemos los valores del config y los 
	// dejamos en las variables 'ip', 'puerto' y 'valor'

	log_info(logger, "Obteniendo los valores del archivo config");

	configMessage = config_get_string_value(config, "CLAVE");
	ip = config_get_string_value(config, "IP");
	puerto = config_get_string_value(config, "PUERTO");

	// Loggeamos el valor de config

	log_info(logger, configMessage);


	/* ---------------- LEER DE CONSOLA ---------------- */

	log_info(
		logger,
		"Todos los mensajes que se escriban a continuacion se loggearan hasta que se escriba un mensaje sin ningun caracter"
	);

	leer_consola(logger);

	/*---------------------------------------------------PARTE 3-------------------------------------------------------------*/

	// ADVERTENCIA: Antes de continuar, tenemos que asegurarnos que el servidor esté corriendo para poder conectarnos a él

	// Creamos una conexión hacia el servidor

	log_info(logger, "Creando la conexion con el servidor");
	socket_client = crear_conexion(ip, puerto, logger);
	log_info(logger, "Conexion creada");

	// Enviamos al servidor el valor de CLAVE como mensaje

	log_info(logger, "Enviando el mensaje que se encuentra en el archivo config");
	enviar_mensaje(configMessage, socket_client);
	log_info(logger, "Mensaje enviado");

	// Armamos y enviamos el paquete

	log_info(
		logger,
		"A partir de este momento, todos los mensajes que se escriban a continuacion se "
		"almacenaran en un paquete. Cuando se escriba un mensaje vacio, "
		"todos los mensajes escritos se enviaran al servidor y se finalizara el programa."
	);

	paquete(socket_client);

	log_info(logger, "Paquete enviado");


	// Finaliza el programa

	log_info(logger, "Cortando la conexion con el servidor y liberando la memoria utilizada. Fianl del programa");
	terminar_programa(socket_client, logger, config);

	return 0;

	/*---------------------------------------------------PARTE 5-------------------------------------------------------------*/
	// Proximamente
}

t_log* iniciar_logger(void)
{
	t_log* nuevo_logger;

	nuevo_logger = log_create("tp0.log", "tp0", true, LOG_LEVEL_INFO);

	return nuevo_logger;
}

t_config* iniciar_config(void)
{
	t_config* nuevo_config;

	nuevo_config = config_create("tp0.config");

	return nuevo_config;
}

void leer_consola(t_log* logger)
{
	char* leido;

	// La primera te la dejo de yapa
	leido = readline("> ");

	// El resto, las vamos leyendo y logueando hasta recibir un string vacío
	while (!is_empty_string(leido))
	{
		log_info(logger, leido);
		free(leido);
		leido = readline("> ");
	}

	// ¡No te olvides de liberar el ultimo readline antes de regresar!
	free(leido);
}

void paquete(int socket_client)
{
	// Ahora toca lo divertido!
	char* leido;
	t_paquete* paquete = crear_paquete();

	// Leemos y esta vez agregamos las lineas al paquete
	leido = readline("> ");

	while (!is_empty_string(leido))
	{
		agregar_a_paquete(paquete, leido, strlen(leido) + 1); // (+ 1) para tener en cuenta el caracter nulo '\0'
		free(leido);
		leido = readline("> ");
	}
	
	enviar_paquete(paquete, socket_client);

	// ¡No te olvides de liberar las líneas y el paquete antes de regresar!
	free(leido);
	eliminar_paquete(paquete);
}

void terminar_programa(int socket_client, t_log* logger, t_config* config)
{
	//Y por ultimo, hay que liberar lo que utilizamos (conexion, log y config) 
	//con las funciones de las commons y del TP mencionadas en el enunciado

	printf("Cerrando el socket_client\n");
	close(socket_client);
	printf("Destruyendo el logger\n");
	log_destroy(logger);
	printf("Destruyendo el config\n");
	config_destroy(config);
}