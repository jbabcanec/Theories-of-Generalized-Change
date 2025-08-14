#!/usr/bin/env ruby

class VectorCalculus
  def self.line_integral_2d(f_field, curve, t_range, n_points = 1000)
    t_min, t_max = t_range
    dt = (t_max - t_min) / n_points.to_f
    integral = 0
    
    n_points.times do |i|
      t = t_min + i * dt
      t_next = t_min + (i + 1) * dt
      
      r = curve.call(t)
      r_next = curve.call(t_next)
      dr = [r_next[0] - r[0], r_next[1] - r[1]]
      
      f_val = f_field.call(r[0], r[1])
      integral += f_val[0] * dr[0] + f_val[1] * dr[1]
    end
    
    integral
  end
  
  def self.double_integral_2d(f, x_range, y_range, nx = 100, ny = 100)
    x_min, x_max = x_range
    y_min, y_max = y_range
    
    dx = (x_max - x_min) / nx.to_f
    dy = (y_max - y_min) / ny.to_f
    integral = 0
    
    nx.times do |i|
      ny.times do |j|
        x = x_min + (i + 0.5) * dx
        y = y_min + (j + 0.5) * dy
        integral += f.call(x, y) * dx * dy
      end
    end
    
    integral
  end
  
  def self.curl_2d(p_func, q_func, x, y, h = 1e-5)
    dQ_dx = (q_func.call(x + h, y) - q_func.call(x - h, y)) / (2 * h)
    dP_dy = (p_func.call(x, y + h) - p_func.call(x, y - h)) / (2 * h)
    dQ_dx - dP_dy
  end
  
  def self.divergence_2d(p_func, q_func, x, y, h = 1e-5)
    dP_dx = (p_func.call(x + h, y) - p_func.call(x - h, y)) / (2 * h)
    dQ_dy = (q_func.call(x, y + h) - q_func.call(x, y - h)) / (2 * h)
    dP_dx + dQ_dy
  end
  
  def self.save_vector_field_analysis(p_func, q_func, x_range, y_range, resolution, filename)
    File.open(filename, 'w') do |file|
      file.puts "x,y,P,Q,divergence,curl,magnitude"
      
      x_min, x_max = x_range
      y_min, y_max = y_range
      
      dx = (x_max - x_min) / resolution.to_f
      dy = (y_max - y_min) / resolution.to_f
      
      (0..resolution).each do |i|
        (0..resolution).each do |j|
          x = x_min + i * dx
          y = y_min + j * dy
          
          p_val = p_func.call(x, y)
          q_val = q_func.call(x, y)
          div = divergence_2d(p_func, q_func, x, y)
          curl = curl_2d(p_func, q_func, x, y)
          magnitude = Math.sqrt(p_val**2 + q_val**2)
          
          file.puts "#{x},#{y},#{p_val},#{q_val},#{div},#{curl},#{magnitude}"
        end
      end
    end
    
    puts "Vector field analysis saved to #{filename}"
  end
end

puts "Vector Calculus Analysis (Ruby version)"
puts "======================================="

# Example vector field F = <-y, x>
p_func = ->(x, y) { -y }
q_func = ->(x, y) { x }

# Verify Green's theorem for unit circle
f_field = ->(x, y) { [p_func.call(x, y), q_func.call(x, y)] }
unit_circle = ->(t) { [Math.cos(t), Math.sin(t)] }

line_integral = VectorCalculus.line_integral_2d(f_field, unit_circle, [0, 2*Math::PI])

curl_func = ->(x, y) { VectorCalculus.curl_2d(p_func, q_func, x, y) }
area_integral = VectorCalculus.double_integral_2d(curl_func, [-1, 1], [-1, 1])

puts "Green's Theorem Verification:"
puts "Line integral: #{line_integral.round(6)}"
puts "Area integral: #{area_integral.round(6)}"
puts "Difference: #{(line_integral - area_integral).abs.round(6)}"

VectorCalculus.save_vector_field_analysis(p_func, q_func, [-3, 3], [-3, 3], 25, "vector_calculus_ruby.csv")