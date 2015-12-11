FILE(REMOVE_RECURSE
  "CMakeFiles/arith_objs"
  "relic_fp_rdc_low.o"
  "relic_fp_cmp_low.o"
  "relic_fp_mul_low.o"
  "relic_fp_add_low.o"
)

# Per-language clean rules from dependency scanning.
FOREACH(lang)
  INCLUDE(CMakeFiles/arith_objs.dir/cmake_clean_${lang}.cmake OPTIONAL)
ENDFOREACH(lang)
