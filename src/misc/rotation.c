#include <stdio.h>
#include <string.h>
#include <math.h>
#include <vlc_rotation.h>

static const char *find_key(const char *haystack, const char *needle, size_t haylen) {
    if (!haystack || !needle){
        return NULL;
    }
    size_t nlen = strlen(needle);
    if (haylen < nlen){
        return NULL;
    }
    for (size_t i=0; i<haylen-nlen; i++) {
      if (memcmp(haystack+i, needle, nlen) == 0) {
        return haystack+i;
      }
    }
    return NULL;
}

static float read_rotation(const char *start) {
  // double, big-endian
  float result = -1;
  double ref;
  char *cref = (char *)(&ref);
  for (int i=0; i<8; i++) {
    cref[i] = start[7-i];
  }
  printf("Rotation logged at %g\n", ref);
  result = (float)ref;
  // sketch of how to use result:
  // double v = fabs(cos(M_PI*r/180.0));
  // printf("Rotation logged and classified as %g %s\n", v, (v>0.5)?"portrait":"landscape");
  return result;
}

float libvlc_rotation_discover(const char *p_buffer, int i_buffer) {
  float rot = -1;
  const char *result = find_key(p_buffer, "rotation", i_buffer);
  if (result!=NULL) {
    int offset = (result-p_buffer)+strlen("rotation")+1;
    if (i_buffer - offset >= 8) {
      rot = read_rotation(p_buffer+offset);
    } 
  }
  return rot;
}

