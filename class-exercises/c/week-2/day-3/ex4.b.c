for (int i = 0; i < 10; i++) {
#ifdef OUTER_LOOP
  printf("inner - result = %d\n", (i * y));
#endif
#ifndef OUTER_LOOP
  printf("innerloop: %d\n", i);
#endif
}