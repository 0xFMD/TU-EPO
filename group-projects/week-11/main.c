#include <stdio.h>

void json_generator(char *name1, int id1, char *name2, int id2) {
  FILE *fh_json = fopen("file1.json", "w");
  if (!fh_json)
    return;

  fprintf(fh_json, "{\n");
  fprintf(fh_json, "\"students\": [\n");

  fprintf(fh_json, "{\"id\": \"%d\",", id1);
  fprintf(fh_json, "\"name\": \"%s\" ", name1);
  fprintf(fh_json, "},\n");

  fprintf(fh_json, "{\"id\": \"%d\",", id2);
  fprintf(fh_json, "\"name\": \"%s\"", name2);
  fprintf(fh_json, "}\n");

  fprintf(fh_json, "  ]\n");
  fprintf(fh_json, "}");

  fclose(fh_json);
}

void yaml_generator(char *name1, int id1, char *name2, int id2) {
  FILE *fh_yaml = fopen("file2.yaml", "w");
  if (!fh_yaml)
    return;

  fprintf(fh_yaml, "students:\n");
  fprintf(fh_yaml, "- id: %d \n", id1);
  fprintf(fh_yaml, "  name: %s \n", name1);

  fprintf(fh_yaml, "- id: %d \n", id2);
  fprintf(fh_yaml, "  name: %s ", name2);

  fclose(fh_yaml);
}

int main() {

  int id1 = 1;
  char *name1 = "FMD";

  int id2 = 2;
  char *name2 = "Bandar";

  json_generator(name1, id1, name2, id2);
  yaml_generator(name1, id1, name2, id2);

  return 0;
}