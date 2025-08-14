#include <stdio.h>
#include <math.h>

typedef struct {
    double popularity;
    double weight;
    double price;
} ProductVector;

ProductVector create_product(double pop, double weight, double price) {
    ProductVector p = {pop, weight, price};
    return p;
}

double dot_product(ProductVector p, ProductVector q) {
    return p.popularity * q.popularity + p.weight * q.weight + p.price * q.price;
}

ProductVector cross_product(ProductVector p, ProductVector q) {
    ProductVector result;
    result.popularity = p.weight * q.price - p.price * q.weight;
    result.weight = p.price * q.popularity - p.popularity * q.price;
    result.price = p.popularity * q.weight - p.weight * q.popularity;
    return result;
}

double magnitude(ProductVector p) {
    return sqrt(p.popularity * p.popularity + p.weight * p.weight + p.price * p.price);
}

void print_product(ProductVector p, const char* name) {
    printf("%s: [%.1f, %.1f, %.1f]\n", name, p.popularity, p.weight, p.price);
}

int main() {
    printf("Product Analysis using Vector Operations (C version)\n");
    printf("===================================================\n");
    
    // Feature vectors for two products: [popularity, weight (kg), price (dollars)]
    ProductVector p = create_product(8, 2, 15);
    ProductVector q = create_product(5, 3, 12);
    
    print_product(p, "Product P");
    print_product(q, "Product Q");
    printf("\n");
    
    // Calculate the dot product
    double dot_prod = dot_product(p, q);
    printf("Dot Product (Similarity Score): %.1f\n", dot_prod);
    
    // Calculate the cross product
    ProductVector cross_prod = cross_product(p, q);
    printf("Cross Product (Directional Difference): ");
    print_product(cross_prod, "");
    
    // Use dot product as a basic similarity measure for recommendation
    double similarity_threshold = 200;
    if (dot_prod > similarity_threshold) {
        printf("Products are similar enough to recommend together.\n");
    } else {
        printf("Products are distinct enough to consider separately.\n");
    }
    
    // Analyze uniqueness with cross product
    double cross_magnitude = magnitude(cross_prod);
    if (cross_magnitude > 0) {
        printf("Products have unique directional attributes across feature space.\n");
    } else {
        printf("Products are very similar across all feature dimensions.\n");
    }
    
    printf("\nDetailed Analysis:\n");
    printf("Dot product interpretation: %.1f\n", dot_prod);
    printf("- This measures overall similarity across all features\n");
    printf("- Higher values indicate more similar products\n");
    
    printf("\nCross product magnitude: %.3f\n", cross_magnitude);
    printf("- This measures how different the products are directionally\n");
    printf("- Higher values indicate more unique feature combinations\n");
    
    // Additional analysis
    printf("\nProduct feature analysis:\n");
    printf("P magnitude (overall strength): %.3f\n", magnitude(p));
    printf("Q magnitude (overall strength): %.3f\n", magnitude(q));
    
    // Normalized dot product (cosine similarity)
    double cosine_similarity = dot_prod / (magnitude(p) * magnitude(q));
    printf("Cosine similarity: %.3f\n", cosine_similarity);
    printf("- Values closer to 1 indicate very similar products\n");
    printf("- Values closer to 0 indicate orthogonal (unrelated) products\n");
    printf("- Values closer to -1 indicate opposite products\n");
    
    return 0;
}