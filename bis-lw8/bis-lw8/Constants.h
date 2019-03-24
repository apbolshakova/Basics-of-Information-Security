#define SRC_FILE_PATH "src.bmp"
#define DEST_FILE_PATH "result.bmp"
#define TEXT_FILE_PATH "text.txt"
#define DECODED_FILE_PATH "decoded.txt"

#define BIN_BASE 2
#define BITS_IN_BYTE 8

#define OFFSET_POS_HEX 0x0A //адрес отступа, начиная с которого идут байты изображения
#define PALETTE_POS_HEX 0x1C //адрес палитры
#define SIZE_POS_HEX 0x22 //адрес размера контейнера

#define PALETTE 24 //размер палитры для проверки корректности файла
#define FILE_SIZE_LEN 4 //количество байт, отвечающих за размер изображения
#define LEN_SIZE 16 //количество бит для хранения длины зашифрованного текста
#define LEN_SIZE_BYTE 8 //количество байт, в которые записывается длина текста
#define PACK_SIZE 3 //количество бит для хранения степени упаковки
#define PACK_SIZE_BYTE 1 //количество байт, в которые записывается степень упаковки
#define MIN_PACK 1
#define MAX_PACK 7