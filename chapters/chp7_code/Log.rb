#!/usr/bin/env ruby

class RiemannSurface
  def self.generate_log_surface(r_range, theta_range, nr, ntheta, filename)
    File.open(filename, 'w') do |file|
      file.puts "x,y,z,real_log,imag_log"
      
      r_min, r_max = r_range
      theta_min, theta_max = theta_range
      
      dr = (r_max - r_min) / nr.to_f
      dtheta = (theta_max - theta_min) / ntheta.to_f
      
      (0..nr).each do |i|
        (0..ntheta).each do |j|
          r = r_min + i * dr
          theta = theta_min + j * dtheta
          
          x = r * Math.cos(theta)
          y = r * Math.sin(theta)
          z = theta
          
          # Complex logarithm: log(z) = ln|z| + i*arg(z)
          complex_z = Complex(x, y)
          log_z = Complex.log(complex_z)
          
          file.puts "#{x},#{y},#{z},#{log_z.real},#{log_z.imag}"
        end
      end
    end
    
    puts "Riemann surface data saved to #{filename}"
  end
end

puts "Riemann Surface for log(z) (Ruby version)"
puts "=========================================="

RiemannSurface.generate_log_surface([0.1, 2], [-4*Math::PI, 4*Math::PI], 100, 200, "riemann_log_ruby.csv")