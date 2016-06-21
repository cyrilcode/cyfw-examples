#include <cyfw/main.h>
#include <cyfw/Gui.h>

using namespace std;
using namespace cy;

namespace Eigen {
    template<typename Scalar>
    Eigen::Matrix<Scalar,4,4> ortho( Scalar const& left,
                                     Scalar const& right,
                                     Scalar const& bottom,
                                     Scalar const& top,
                                     Scalar const& zNear,
                                     Scalar const& zFar ) {
        Eigen::Matrix<Scalar,4,4> mat = Eigen::Matrix<Scalar,4,4>::Identity();
        mat(0,0) = Scalar(2) / (right - left);
        mat(1,1) = Scalar(2) / (top - bottom);
        mat(2,2) = - Scalar(2) / (zFar - zNear);
        mat(3,0) = - (right + left) / (right - left);
        mat(3,1) = - (top + bottom) / (top - bottom);
        mat(3,2) = - (zFar + zNear) / (zFar - zNear);
        return mat.transpose();
    }
}

class Effect
{
    mat4f projection;

public:
    int width, height;
    Shader shader;
    GLuint texture;
    GLuint fbo;

    string effectCode;

    ~Effect()
    {
//        glDeleteTextures(1, &texture);
//        glDeleteFramebuffers(1, &fbo);
    }

    Effect(int w, int h, string c) : width{w}, height{h}, effectCode(c)
    {
        shader = {};
        shader.init("example",glsl(
                in vec2 position;
                uniform mat4 projection;
                void main()
                {
                    gl_Position = projection * vec4(position, 0.0f, 1.0f);
                }
        ), glsl(
                out vec4 fragColor;
                uniform vec2 resolution;
                uniform float globalTime;
                   void mainImage(out vec4 c, in vec2 p);
                void main()
                {
//                    vec2 uv = gl_FragCoord.xy / resolution;
//                    fragColor = vec4(uv,0.5+0.5*sin(0.0),1.0);
//                    vec2 uv = floor(5 * gl_FragCoord.xy * vec2(resolution.x / resolution.y, 1) / resolution.xy);
//                    fragColor = vec4(vec3(mod(uv.x + uv.y, 2.)), 1);
                    mainImage(fragColor, gl_FragCoord.xy);
                }
        ) + effectCode);

        mat_u indices(3, 2);
        indices.col(0) << 0, 1, 3;
        indices.col(1) << 1, 2, 3;

        mat_f positions(2, 4);
        positions.col(0) <<  width, height;
        positions.col(1) <<  width,   0.0f;
        positions.col(2) <<   0.0f,   0.0f;
        positions.col(3) <<   0.0f, height;

        shader.bind();
        shader.uploadIndices(indices);
        shader.uploadAttrib("position", positions);

        glGenFramebuffers(1, &fbo);
        glBindFramebuffer(GL_FRAMEBUFFER, fbo);
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);

        projection = Eigen::ortho(0.0f, float(width), float(height), 0.0f, -1.0f, 1.0f);
    }

    void update()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, fbo);
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);

        glBindTexture(GL_TEXTURE_2D, texture);
        shader.bind();
        shader.setUniform("resolution", vec2f{width, height}, false);
        shader.setUniform("projection", projection, false);
        shader.setUniform("globalTime", static_cast<float>(cy::time()), false);
        shader.drawIndexed(shader::mode::TRIANGLES, 0, 2);

        glBindFramebuffer(GL_FRAMEBUFFER, 0); // back to default
    }

};





class MyApp : public cy::App
{
    Gui gui;
    Effect effect;

    int dims[2];


    char text[1024*16];

public:
    MyApp() : dims{640, 480}, effect{640, 480, " void mainImage(out vec4 c, in vec2 p){\nc = vec4(1,0,0,1);\n}\n"} {

        char * t = "void mainImage(out vec4 c, in vec2 p){\nc = vec4(1,0,0,1);\n}\n\0";
        memcpy(text, t, 62);
    }

    void setup()
    {
        gui.init(window);
        window->setClearColor({0,0,0});
    }

    void draw()
    {

        window->clear();
        gui.clear();

        gui.demo();

        ImGui::InputInt2("output size", dims);

        if (ImGui::Button("Resize"))
        {
            try
            {
                effect = Effect{dims[0], dims[1], " " + string(text)};
            }
            catch (string err)
            {
                cout << err << endl;
            }
        }

        effect.update();
        ImGui::Image((void*)effect.texture, ImVec2(320, (320.0f / effect.width) * effect.height));

        ImGui::InputTextMultiline("source", text, sizeof(text), ImVec2(-1.0f, -1.0f), 0);

        gui.draw();
    }

    void key(window::KeyEvent e) { gui.key(e); }
    void scroll(window::ScrollEvent e) { gui.scroll(e); }
    void textInput(window::CharEvent e) { gui.character(e); }
    void mouseButton(window::MouseButtonEvent e ) { gui.mouse(e); }
};

int main() {
    cy::run<MyApp,ConsoleLogger>(800, 900, "multiple editors");
}


/**
Here's a test program you can copy and paste into the shader editor...




 const float eps = 0.05;
const int maxIterations = 128;
const float stepScale = 0.5;
const float stopThreshold = 0.005;

float sphere(in vec3 p, in vec3 centerPos, float radius) {
  return length(p-centerPos) - radius;
}

float bumps(in vec3 p){
    return sin(p.x*16.+ globalTime *0.57)
        *cos(p.y*16.+globalTime*2.17)
        *sin(p.z*16.-globalTime*1.31)
        + 0.5*sin(p.x*32.+globalTime*0.07)
        *cos(p.y*32.+globalTime*2.11)
        *sin(p.z*32.-globalTime*1.23);
}

float torus( vec3 p, vec2 t )
{
  vec2 q = vec2(length(p.xz)-t.x,p.y);
  return length(q)-t.y;
}

float roundedCube(vec3 p, vec3 boxExtents, float edgeRadius ){
return length(max(abs(p)-boxExtents + vec3(edgeRadius), 0.0 )) - edgeRadius;
}


float scene(in vec3 p) {
    p = mod(p, 1.0) - 0.5;
    return torus(p, vec2(0.5,0.2));
}


float rayMarching( vec3 origin, vec3 dir, float start, float end ) {

float sceneDist = 1e4;
float rayDepth = start;
for ( int i = 0; i < maxIterations; i++ ) {

sceneDist = scene( origin + dir * rayDepth );
if (( sceneDist < stopThreshold ) || (rayDepth >= end)) {
break;
}
rayDepth += sceneDist * stepScale;

}
if ( sceneDist >= stopThreshold ) rayDepth = end;
else rayDepth += sceneDist;
return rayDepth;
}

vec3 getNormal(in vec3 p) {
return normalize(vec3(
scene(vec3(p.x+eps,p.y,p.z))-scene(vec3(p.x-eps,p.y,p.z)),
scene(vec3(p.x,p.y+eps,p.z))-scene(vec3(p.x,p.y-eps,p.z)),
scene(vec3(p.x,p.y,p.z+eps))-scene(vec3(p.x,p.y,p.z-eps))
));
}


vec4 getColor(in vec2 fragCoord)
{

    vec2 aspect = vec2( resolution.x/resolution.y, 1.0);
vec2 screenCoords = (2.0*fragCoord.xy/resolution.xy - 1.0)*aspect;


vec3 lookAt = vec3(0.,sin(globalTime / 3.0),cos(globalTime / 4.0));
vec3 camPos = vec3(cos(globalTime / 2.0),0.,sin(globalTime) - 2.0);

    // Camera setup.
    vec3 forward = normalize(lookAt-camPos);
    vec3 right = normalize(vec3(forward.z, 0., -forward.x ));
    vec3 up = normalize(cross(forward,right));
    float FOV = 0.5;

    // Ray
    vec3 ro = camPos;
    vec3 rd = normalize(forward + FOV*screenCoords.x*right + FOV*screenCoords.y*up);


    vec3 bgcolor = vec3(0.,0.,0.);
    //vec3 bgcolor = vec3(1.,0.97,0.92)*0.35;
    //float bgshade = (1.0-length(vec2(screenCoords.x/aspect.x, screenCoords.y+0.5) )*0.9);
//bgcolor *= bgshade;

    const float clipNear = 0.0;
const float clipFar = 10.0;
float dist = rayMarching(ro, rd, clipNear, clipFar );
if ( dist >= clipFar ) {
    return vec4(bgcolor, 1.0);
}


    // made it this far, then we've hit something.
    // use dist to color...
vec3 sp = ro + rd*dist;
    vec3 surfNormal = getNormal(sp);


//vec3 lp = vec3(1.5*sin(iGlobalTime*0.5), 0.75+0.25*cos(iGlobalTime*0.5), -1.0);
    vec3 lp = camPos;
vec3 ld = lp-sp;
vec3 lcolor = vec3(1.,0.97,0.92);


float len = length( ld );
ld /= len;
float lightAtten = min( 1.0 / ( 0.25*len*len ), 1.0 );

    vec3 ref = reflect(-ld, surfNormal);
vec3 objColor = vec3(1.,1.,1.);
    if (mod(sp*0.5, 1.0).x < 0.5) objColor = vec3(1, 0.412, 0.706);
vec3 sceneColor = vec3(0.0);


float ambient = .1;
float specularPower = 16.0;
float diffuse = max( 0.0, dot(surfNormal, ld) );
float specular = max( 0.0, dot( ref, normalize(camPos-sp)) );
specular = pow(specular, specularPower);

sceneColor += (objColor*(diffuse*0.8+ambient)+specular*0.5)*lcolor*lightAtten;

return vec4(clamp(sceneColor, 0.0, 1.0), 1.0);
}


void mainImage( out vec4 fragColor, in vec2 fragCoord ) {

    // commented out to use AA version below
   //fragColor = getColor(fragCoord);
    //return;

    // anti aliasing
    float diff = 0.5;
    vec4 c = getColor(fragCoord);
    c += getColor(vec2(fragCoord.x + diff, fragCoord.y));
    c += getColor(vec2(fragCoord.x - diff, fragCoord.y));
    c += getColor(vec2(fragCoord.x, fragCoord.y + diff));
    c += getColor(vec2(fragCoord.x, fragCoord.y - diff));
    fragColor = c / 5.0;
}


*/