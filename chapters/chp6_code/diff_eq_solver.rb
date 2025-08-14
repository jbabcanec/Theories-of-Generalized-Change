#!/usr/bin/env ruby

class ODESolver
  def self.euler_method(f, t0, y0, h, t_final)
    solution = []
    t, y = t0, y0
    
    while t <= t_final
      solution << [t, y]
      y = y + h * f.call(t, y)
      t = t + h
    end
    
    solution
  end
  
  def self.rk4_method(f, t0, y0, h, t_final)
    solution = []
    t, y = t0, y0
    
    while t <= t_final
      solution << [t, y]
      
      k1 = h * f.call(t, y)
      k2 = h * f.call(t + h/2, y + k1/2)
      k3 = h * f.call(t + h/2, y + k2/2)
      k4 = h * f.call(t + h, y + k3)
      
      y = y + (k1 + 2*k2 + 2*k3 + k4) / 6
      t = t + h
    end
    
    solution
  end
  
  def self.save_solution(solution, filename)
    File.open(filename, 'w') do |file|
      file.puts "t,y"
      solution.each { |point| file.puts "#{point[0]},#{point[1]}" }
    end
    puts "Solution saved to #{filename}"
  end
end

puts "ODE Solver Examples (Ruby version)"
puts "=================================="

# Example: y' = -2y + 1, y(0) = 0
ode_func = ->(t, y) { -2*y + 1 }

euler_sol = ODESolver.euler_method(ode_func, 0, 0, 0.1, 2)
rk4_sol = ODESolver.rk4_method(ode_func, 0, 0, 0.1, 2)

ODESolver.save_solution(euler_sol, "euler_solution_ruby.csv")
ODESolver.save_solution(rk4_sol, "rk4_solution_ruby.csv")