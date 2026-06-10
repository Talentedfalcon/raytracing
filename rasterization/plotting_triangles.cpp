#include <iostream>
#include <fstream>
#include <vector>

struct Pixel{
    unsigned char r,g,b;
};

template <typename T=float>
class Vec3{
    public:
        T x,y,z;
        Vec3(){
            x=y=z=0;
        }
        Vec3(T x, T y, T z){
            this->x=x;
            this->y=y;
            this->z=z;
        }
};

class Triangle{
    public:
        Vec3<float> vertex[3];
        Triangle(Vec3<float> A, Vec3<float> B, Vec3<float> C){
            this->vertex[0]=A;
            this->vertex[1]=B;
            this->vertex[2]=C;
        }
        
        friend std::ostream& operator <<(std::ostream& os, const Triangle& obj);
};

std::ostream& operator<<(std::ostream& os, const Triangle& obj) {
    os << "Point A: (" << obj.vertex[0].x << ", " << obj.vertex[0].y << ", " << obj.vertex[0].z << ")\n";
    os << "Point B: (" << obj.vertex[1].x << ", " << obj.vertex[1].y << ", " << obj.vertex[1].z << ")\n";
    os << "Point C: (" << obj.vertex[2].x << ", " << obj.vertex[2].y << ", " << obj.vertex[2].z << ")\n";
    return os; 
}

int render(std::vector<Pixel> image,int image_width,int image_height){
    std::ofstream outFile("output.ppm",std::ios::binary);
    
    try{
        if(!outFile){
            std::cerr << "Error: Could not open file for writing" << std::endl;
            return 1;
        }
        outFile << "P6\n" << image_width << " " << image_height << "\n255\n";
    
        for(int y=0;y<image_height;y++){
            for(int x=0;x<image_width;x++){
                int index=y*image_width+x;
                outFile.put(image[index].r);
                outFile.put(image[index].g);
                outFile.put(image[index].b);
            }
        }
        outFile.close();
    
        std::cout << "Image generated successfully as 'output.ppm'" << std::endl;
    }
    catch(...){
        std::cout << "Caught an unhandled exception\n";
    }
    return 0;
}

int main(){
    const unsigned int image_width=1920,image_height=1080;
    float aspect_ratio=image_width/image_height;

    std::vector<Triangle> objects;

    Triangle t1(Vec3<>(2,1,-5),Vec3<>(3,2,-5),Vec3<>(1,3,-5));
    Triangle t2(Vec3<>(2,1,-5),Vec3<>(1,-2,-5),Vec3<>(1,3,-5));
    Triangle t3(Vec3<>(2,1,-5),Vec3<>(1,-2,-5),Vec3<>(3,0,-5));
    Triangle t4(Vec3<>(2,1,-5),Vec3<>(3,2,-5),Vec3<>(3,0,-5));

    objects.push_back(t1);
    objects.push_back(t2);
    objects.push_back(t3);
    objects.push_back(t4);

    std::vector<Pixel> image(image_width*image_height);

    for(int y=0;y<image_height;y++){
        for(int x=0;x<image_width;x++){
            int index=y*image_width+x;
            image[index].r=0;
            image[index].b=0;
            image[index].g=0;
        }
    }

    for(auto& t: objects){
        // std::cout<<t;

        std::vector<std::pair<int,int>> proj_points;

        for (int i=0;i<3;i++){
            float x_proj=t.vertex[i].x/-t.vertex[i].z;
            float y_proj=t.vertex[i].y/-t.vertex[i].z*aspect_ratio;
            // printf("Projected corner %d: x:%f, y:%f\n",i,x_proj,y_proj);

            float x_proj_remap=(x_proj+1)/2;
            float y_proj_remap=(y_proj+1)/2;
            // printf("\t\tx_remap:%f, y_remap:%f\n",x_proj_remap,y_proj_remap);

            //Raster Space
            int x_proj_pix=x_proj_remap*image_width;
            int y_proj_pix=y_proj_remap*image_height;
            // printf("\t\tx_pix:%d, y_pix:%d\n",x_proj_pix,y_proj_pix);

            int proj_index=y_proj_pix*image_width+x_proj_pix;
            image[proj_index].r=255;

            proj_points.push_back({x_proj_pix,y_proj_pix});
        }

        for(int i=0;i<proj_points.size();i++){
            for(int j=i+1;j<proj_points.size();j++){
                float y_diff=proj_points[j].second-proj_points[i].second;
                float x_diff=proj_points[j].first-proj_points[i].first;
                float m;
                float c;
                bool isInvert=false;
                if(x_diff==0){
                    isInvert=true;
                    m=0;
                    c=proj_points[i].first;
                }
                else{
                    m=y_diff/x_diff;
                    c=(-m*proj_points[i].first)+proj_points[i].second;
                }
                // std::cout << m << " " << c << " " << isInvert << std::endl;

                int max_x=proj_points[j].first;
                int min_x=proj_points[i].first;
                if(proj_points[i].first>proj_points[j].first){
                    max_x=proj_points[i].first;
                    min_x=proj_points[j].first;
                }
                
                int max_y=proj_points[j].second;
                int min_y=proj_points[i].second;
                if(proj_points[i].second>proj_points[j].second){
                    max_y=proj_points[i].second;
                    min_y=proj_points[j].second;
                }

                for(int y=0;y<image_height;y++){
                    for(int x=0;x<image_width;x++){
                        if((x<=max_x && y<=max_y) && (x>=min_x && y>=min_y)){
                            if((!isInvert && y==int(m*x+c)||(isInvert && x==int(m*y+c)))){
                                int index=y*image_width+x;
                                image[index].b=255;
                            }
                        }
                    }
                }
            }
        }
    }

    render(image,image_width,image_height);

    return 0;
}