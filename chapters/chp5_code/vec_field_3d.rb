#!/usr/bin/env ruby

class VectorField3D
  def initialize(fx, fy, fz)
    @fx = fx
    @fy = fy  
    @fz = fz
    @h = 1e-5
  end
  
  def evaluate(x, y, z)
    [@fx.call(x, y, z), @fy.call(x, y, z), @fz.call(x, y, z)]
  end
  
  def divergence(x, y, z)
    dFx_dx = (@fx.call(x + @h, y, z) - @fx.call(x - @h, y, z)) / (2 * @h)
    dFy_dy = (@fy.call(x, y + @h, z) - @fy.call(x, y - @h, z)) / (2 * @h)
    dFz_dz = (@fz.call(x, y, z + @h) - @fz.call(x, y, z - @h)) / (2 * @h)
    dFx_dx + dFy_dy + dFz_dz
  end
  
  def curl(x, y, z)
    dFz_dy = (@fz.call(x, y + @h, z) - @fz.call(x, y - @h, z)) / (2 * @h)
    dFy_dz = (@fy.call(x, y, z + @h) - @fy.call(x, y, z - @h)) / (2 * @h)
    
    dFx_dz = (@fx.call(x, y, z + @h) - @fx.call(x, y, z - @h)) / (2 * @h)
    dFz_dx = (@fz.call(x + @h, y, z) - @fz.call(x - @h, y, z)) / (2 * @h)
    
    dFy_dx = (@fy.call(x + @h, y, z) - @fy.call(x - @h, y, z)) / (2 * @h)
    dFx_dy = (@fx.call(x, y + @h, z) - @fx.call(x, y - @h, z)) / (2 * @h)
    
    [dFz_dy - dFy_dz, dFx_dz - dFz_dx, dFy_dx - dFx_dy]
  end
  
  def save_field_data(x_range, y_range, z_range, resolution, filename)
    File.open(filename, 'w') do |file|
      file.puts "x,y,z,Fx,Fy,Fz,magnitude,divergence,curl_x,curl_y,curl_z"
      
      x_min, x_max = x_range
      y_min, y_max = y_range
      z_min, z_max = z_range
      
      dx = (x_max - x_min) / resolution.to_f
      dy = (y_max - y_min) / resolution.to_f
      dz = (z_max - z_min) / resolution.to_f
      
      (0..resolution).each do |i|
        (0..resolution).each do |j|
          (0..resolution).each do |k|
            x = x_min + i * dx
            y = y_min + j * dy
            z = z_min + k * dz
            
            field = evaluate(x, y, z)
            magnitude = Math.sqrt(field[0]**2 + field[1]**2 + field[2]**2)
            div = divergence(x, y, z)
            curl_vec = curl(x, y, z)
            
            file.puts "#{x},#{y},#{z},#{field[0]},#{field[1]},#{field[2]},#{magnitude},#{div},#{curl_vec[0]},#{curl_vec[1]},#{curl_vec[2]}"
          end
        end
      end
    end
    
    puts "3D vector field data written to #{filename}"
  end
end

puts "3D Vector Field Analysis (Ruby version)"
puts "======================================="

# Example: F = <y², 2xy, x+z>
field = VectorField3D.new(
  ->(x, y, z) { y**2 },
  ->(x, y, z) { 2*x*y },
  ->(x, y, z) { x + z }
)

field.save_field_data([-2, 2], [-2, 2], [-2, 2], 5, "vector_field_3d_ruby.csv")