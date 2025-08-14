#!/usr/bin/env ruby

class ProductVector
  attr_accessor :popularity, :weight, :price
  
  def initialize(popularity, weight, price)
    @popularity = popularity.to_f
    @weight = weight.to_f  
    @price = price.to_f
  end
  
  def dot_product(other)
    @popularity * other.popularity + @weight * other.weight + @price * other.price
  end
  
  def cross_product(other)
    ProductVector.new(
      @weight * other.price - @price * other.weight,
      @price * other.popularity - @popularity * other.price,
      @popularity * other.weight - @weight * other.popularity
    )
  end
  
  def magnitude
    Math.sqrt(@popularity**2 + @weight**2 + @price**2)
  end
  
  def to_s
    "[#{@popularity}, #{@weight}, #{@price}]"
  end
end

# Feature vectors for two products: [popularity, weight (kg), price (dollars)]
p = ProductVector.new(8, 2, 15)
q = ProductVector.new(5, 3, 12)

puts "Product Analysis using Vector Operations"
puts "======================================="
puts "Product P: #{p}"
puts "Product Q: #{q}"
puts

# Calculate the dot product
dot_product = p.dot_product(q)
puts "Dot Product (Similarity Score): #{dot_product}"

# Calculate the cross product
cross_product = p.cross_product(q)
puts "Cross Product (Directional Difference): #{cross_product}"

# Use dot product as a basic similarity measure for recommendation
similarity_threshold = 200
if dot_product > similarity_threshold
  puts "Products are similar enough to recommend together."
else
  puts "Products are distinct enough to consider separately."
end

# Analyze uniqueness with cross product
if cross_product.magnitude > 0
  puts "Products have unique directional attributes across feature space."
else
  puts "Products are very similar across all feature dimensions."
end

puts "\nDetailed Analysis:"
puts "Dot product interpretation: #{dot_product}"
puts "- This measures overall similarity across all features"
puts "- Higher values indicate more similar products"

puts "\nCross product magnitude: #{cross_product.magnitude.round(3)}"
puts "- This measures how different the products are directionally"
puts "- Higher values indicate more unique feature combinations"

# Additional analysis
puts "\nProduct feature analysis:"
puts "P magnitude (overall strength): #{p.magnitude.round(3)}"
puts "Q magnitude (overall strength): #{q.magnitude.round(3)}"

# Normalized dot product (cosine similarity)
cosine_similarity = dot_product / (p.magnitude * q.magnitude)
puts "Cosine similarity: #{cosine_similarity.round(3)}"
puts "- Values closer to 1 indicate very similar products"
puts "- Values closer to 0 indicate orthogonal (unrelated) products"
puts "- Values closer to -1 indicate opposite products"