#!/usr/bin/env ruby

class Vector3D
  attr_accessor :x, :y, :z
  
  def initialize(x = 0, y = 0, z = 0)
    @x, @y, @z = x.to_f, y.to_f, z.to_f
  end
  
  def magnitude
    Math.sqrt(@x**2 + @y**2 + @z**2)
  end
  
  def normalize
    mag = magnitude
    return Vector3D.new(0, 0, 0) if mag == 0
    Vector3D.new(@x / mag, @y / mag, @z / mag)
  end
  
  def dot(other)
    @x * other.x + @y * other.y + @z * other.z
  end
  
  def cross(other)
    Vector3D.new(
      @y * other.z - @z * other.y,
      @z * other.x - @x * other.z,
      @x * other.y - @y * other.x
    )
  end
  
  def project_onto(other)
    scalar = self.dot(other) / other.dot(other)
    Vector3D.new(scalar * other.x, scalar * other.y, scalar * other.z)
  end
  
  def angle_with(other)
    cos_angle = self.dot(other) / (self.magnitude * other.magnitude)
    cos_angle = [[-1.0, cos_angle].max, 1.0].min  # Clamp to [-1, 1]
    Math.acos(cos_angle)
  end
  
  def to_s
    "[#{@x.round(3)}, #{@y.round(3)}, #{@z.round(3)}]"
  end
end

def triple_scalar_product(a, b, c)
  a.dot(b.cross(c))
end

# Example usage
puts "Vector Operations in Ruby"
puts "========================="

# Define three vectors
v1 = Vector3D.new(1, 2, 3)
v2 = Vector3D.new(4, -1, 2)
v3 = Vector3D.new(2, 3, -1)

puts "Vector v1: #{v1}"
puts "Vector v2: #{v2}"
puts "Vector v3: #{v3}"

# Calculate various operations
dot_product = v1.dot(v2)
cross_product = v1.cross(v2)
projection = v1.project_onto(v2)
angle = v1.angle_with(v2)
triple = triple_scalar_product(v1, v2, v3)

puts "\nOperations:"
puts "Dot product v1·v2: #{dot_product.round(3)}"
puts "Cross product v1×v2: #{cross_product}"
puts "Projection of v1 onto v2: #{projection}"
puts "Angle between v1 and v2: #{(angle * 180.0 / Math::PI).round(3)} degrees"
puts "Triple scalar product: #{triple.round(3)}"
puts "Volume of parallelepiped: #{triple.abs.round(3)}"

# Additional examples
puts "\nAdditional Examples:"

# Unit vectors
i_hat = Vector3D.new(1, 0, 0)
j_hat = Vector3D.new(0, 1, 0)
k_hat = Vector3D.new(0, 0, 1)

puts "Unit vectors:"
puts "i_hat × j_hat = #{i_hat.cross(j_hat)}"
puts "j_hat × k_hat = #{j_hat.cross(k_hat)}"
puts "k_hat × i_hat = #{k_hat.cross(i_hat)}"

# Orthogonality test
perpendicular = Vector3D.new(3, -2, 1)
test_vector = Vector3D.new(1, 1, 1)

puts "\nOrthogonality test:"
puts "#{perpendicular} · #{test_vector} = #{perpendicular.dot(test_vector)}"
puts "Vectors are #{'not ' unless perpendicular.dot(test_vector).abs < 1e-10}orthogonal"