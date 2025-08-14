#!/usr/bin/env ruby

class HeatEquation
  def self.solve_1d(initial_condition, length, time, alpha, nx = 100, nt = 1000)
    dx = length / (nx - 1).to_f
    dt = time / nt.to_f
    r = alpha * dt / (dx**2)
    
    puts "Warning: r = #{r.round(3)} > 0.5, solution may be unstable" if r > 0.5
    
    # Initialize solution array
    u = Array.new(nt) { Array.new(nx, 0) }
    
    # Set initial condition
    nx.times do |i|
      x = i * dx
      u[0][i] = initial_condition.call(x)
    end
    
    # Boundary conditions (fixed at zero)
    nt.times do |n|
      u[n][0] = 0
      u[n][nx-1] = 0
    end
    
    # Time stepping
    (nt-1).times do |n|
      (1...nx-1).each do |i|
        u[n+1][i] = u[n][i] + r * (u[n][i+1] - 2*u[n][i] + u[n][i-1])
      end
    end
    
    u
  end
  
  def self.save_solution(u, length, time, filename)
    File.open(filename, 'w') do |file|
      file.puts "x,t,u"
      
      nt = u.size
      nx = u[0].size
      
      (0...nt).step(nt/50) do |n| # Sample every 50th time step
        nx.times do |i|
          x = i * length / (nx - 1).to_f
          t = n * time / (nt - 1).to_f
          file.puts "#{x},#{t},#{u[n][i]}"
        end
      end
    end
    
    puts "Heat equation solution saved to #{filename}"
  end
end

puts "Heat Equation Solver (Ruby version)"
puts "==================================="

# Initial condition: sin(πx/L)
initial_temp = ->(x) { Math.sin(Math::PI * x / 10.0) }

solution = HeatEquation.solve_1d(initial_temp, 10.0, 2.0, 0.5)
HeatEquation.save_solution(solution, 10.0, 2.0, "heat_solution_ruby.csv")