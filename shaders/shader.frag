#version 410 core

uniform vec2 resolution;
uniform float time;
uniform uint frame;

out vec4 col;

#define MAX_STEPS 100
#define MAX_DIST 100.0
#define MIN_DIST 0.01


float getDist(vec3 p) {
    vec3 spherePos = vec3(0, 1, 6);
    // r = radius of the sphere
    float r = 1.0;
    float dist = length(p - spherePos) - r;
    float pDist = p.y;
    float minD = min(dist, pDist);
    
    return minD;
}


float rayMarch(vec3 ro, vec3 rd) {
    float d = 0.0;
    for (int i = 0;i < MAX_STEPS; i++) {
        vec3 p = ro + d * rd;
        float dist = getDist(p);
        d += dist;
        if (dist < MIN_DIST || d > MAX_DIST) break;
    }
    
    return d;
}

vec3 getNormal(vec3 p) {
    float d = getDist(p);
    vec2 e = vec2(.01, 0.0);
    // calculates the normal based on a point on the surface hit close to the point of incidence
    vec3 n = d - vec3(
        getDist(p - e.xyy),
        getDist(p - e.yxy),
        getDist(p - e.yyx)
     );
     
     return normalize(n);
}

float getLight(vec3 p) {
    vec3 pos = vec3(0, 5, 6);
    pos += vec3(sin(time), 0.0, cos(time));
    // l = direction to light from point of incidence
    vec3 l  = normalize(pos - p);
    vec3 n = getNormal(p);
    
    float diff = clamp(dot(n, l), 0.0, 1.0);
    // d = distance to light from point of incidence + a small amount
    float d = rayMarch(p + n * MIN_DIST * 2.0  , l);
    // if d smaller than distance to light from point of incidence
    // we hit an object before reaching the light, thus we are in shadow
    if (d < length(pos - p)) diff *= 0.4;
    return diff;
}


void main () {
    // normalized uv's
    vec2 uv = gl_FragCoord.xy / resolution;
    float aspect = resolution.x / resolution.y;
    // Shifting uv's over to have (0, 0) in the middle of the screen
    uv -= 0.5;
    // scaling x component properly according to the view ports aspect ratio
    uv.x *= aspect;

    // ray orign and ray direction
    vec3 ro = vec3(0, 1, 0);
    vec3 rd = normalize(vec3(uv, 1.0));
    
    // d = distance travelled until sphere is hit
    float d = rayMarch(ro, rd);
    vec3 p = ro + d * rd;
    float diff = getLight(p);

    col = vec4(vec3(diff), 1.0);
}
