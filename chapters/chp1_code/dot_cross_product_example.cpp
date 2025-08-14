#include <iostream>
#include <vector>
#include <cmath>
#include <iomanip>

class ProductVector {
public:
    double popularity;
    double weight;
    double price;
    
    ProductVector(double pop, double w, double pr) 
        : popularity(pop), weight(w), price(pr) {}
    
    double dot_product(const ProductVector& other) const {
        return popularity * other.popularity + 
               weight * other.weight + 
               price * other.price;
    }
    
    ProductVector cross_product(const ProductVector& other) const {
        return ProductVector(
            weight * other.price - price * other.weight,
            price * other.popularity - popularity * other.price,
            popularity * other.weight - weight * other.popularity
        );
    }
    
    double magnitude() const {
        return std::sqrt(popularity * popularity + 
                        weight * weight + 
                        price * price);
    }
    
    void print() const {
        std::cout << "[" << popularity << ", " << weight << ", " << price << "]";
    }
};

int main() {
    // Feature vectors for two products: [popularity, weight (kg), price (dollars)]
    ProductVector p(8, 2, 15);
    ProductVector q(5, 3, 12);
    
    // Calculate the dot product
    double dot_product = p.dot_product(q);
    std::cout << "Dot Product (Similarity Score): " << dot_product << std::endl;
    
    // Calculate the cross product
    ProductVector cross_product = p.cross_product(q);
    std::cout << "Cross Product (Directional Difference): ";
    cross_product.print();
    std::cout << std::endl;
    
    // Use dot product as a basic similarity measure for recommendation
    double similarity_threshold = 200;
    if (dot_product > similarity_threshold) {
        std::cout << "Products are similar enough to recommend together." << std::endl;
    } else {
        std::cout << "Products are distinct enough to consider separately." << std::endl;
    }
    
    // Analyze uniqueness with cross product
    if (cross_product.magnitude() > 0) {
        std::cout << "Products have unique directional attributes across feature space." << std::endl;
    } else {
        std::cout << "Products are very similar across all feature dimensions." << std::endl;
    }
    
    return 0;
}