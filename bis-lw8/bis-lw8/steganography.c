int main(void)
{
	if (!encode()) printf("Encoding error.\n");
	else
	{
        printf("Successfully encoded.\n");
		if (!decode()) printf("Decoding error.\n");
		else printf("Successfully decoded.\n");
	}
	_getch();
	return 0;
}