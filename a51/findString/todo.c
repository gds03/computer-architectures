String SO = “gonçalo”;
String SF = “alo”;

static int main()
{
	for (int i = 0; i < SO.length; i++){
		if (SO.charAt(i) == SF.charAt(0))
			if (checkIfIs(i))
				return i;
	}
}
boolean checkIfIs(int id)
{
	int count = 0;
	for (int i = 0; count != SF.length && SF.charAt(i) == SO.charAt(id); count++, id++, i++);
	return count == SF.length;
}

