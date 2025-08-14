#!/usr/bin/env ruby

class VectorField2D
  def initialize(p_func, q_func)
    @P = p_func
    @Q = q_func
    @h = 1e-5
  end
  
  def evaluate(x, y)
    [@P.call(x, y), @Q.call(x, y)]
  end
  
  def divergence(x, y)
    dP_dx = (@P.call(x + @h, y) - @P.call(x - @h, y)) / (2 * @h)
    dQ_dy = (@Q.call(x, y + @h) - @Q.call(x, y - @h)) / (2 * @h)
    dP_dx + dQ_dy
  end
  
  def curl(x, y)
    dQ_dx = (@Q.call(x + @h, y) - @Q.call(x - @h, y)) / (2 * @h)
    dP_dy = (@P.call(x, y + @h) - @P.call(x, y - @h)) / (2 * @h)
    dQ_dx - dP_dy
  end
  
  def save_field_data(x_range, y_range, resolution, filename)
    File.open(filename, 'w') do |file|
      file.puts "x,y,P,Q,magnitude,divergence,curl"
      
      x_min, x_max = x_range
      y_min, y_max = y_range
      
      dx = (x_max - x_min) / resolution.to_f
      dy = (y_max - y_min) / resolution.to_f
      
      (0..resolution).each do |i|
        (0..resolution).each do |j|
          x = x_min + i * dx
          y = y_min + j * dy
          
          field = evaluate(x, y)
          magnitude = Math.sqrt(field[0]**2 + field[1]**2)
          div = divergence(x, y)
          curl_val = curl(x, y)
          
          file.puts "#{x},#{y},#{field[0]},#{field[1]},#{magnitude},#{div},#{curl_val}"
        end
      end
    end
    
    puts "Vector field data written to #{filename}"
  end
end

puts "2D Vector Field Analysis (Ruby version)"
puts "======================================="

# Example 1: Circulation field F = <-y, x>
circulation_field = VectorField2D.new(
  ->(x, y) { -y },
  ->(x, y) { x }
)

puts "Example 1: Circulation field F = <-y, x>"
circulation_field.save_field_data([-10, 10], [-10, 10], 25, "circulation_field_ruby.csv")

# Example 2: Radial field F = <x, y>
radial_field = VectorField2D.new(
  ->(x, y) { x },
  ->(x, y) { y }
)

puts "Example 2: Radial field F = <x, y>"
radial_field.save_field_data([-5, 5], [-5, 5], 25, "radial_field_ruby.csv")