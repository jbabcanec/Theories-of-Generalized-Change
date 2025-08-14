#!/usr/bin/env ruby

class Vector3D
  attr_accessor :x, :y, :z
  
  def initialize(x, y, z)
    @x, @y, @z = x.to_f, y.to_f, z.to_f
  end
  
  def magnitude
    Math.sqrt(@x**2 + @y**2 + @z**2)
  end
  
  def -(other)
    Vector3D.new(@x - other.x, @y - other.y, @z - other.z)
  end
  
  def +(other)
    Vector3D.new(@x + other.x, @y + other.y, @z + other.z)
  end
  
  def *(scalar)
    Vector3D.new(@x * scalar, @y * scalar, @z * scalar)
  end
  
  def cross(other)
    Vector3D.new(
      @y * other.z - @z * other.y,
      @z * other.x - @x * other.z,
      @x * other.y - @y * other.x
    )
  end
  
  def to_s
    "(#{@x.round(3)}, #{@y.round(3)}, #{@z.round(3)})"
  end
end

class ParametricCurve
  def initialize(&block)
    @curve = block
    @dt = 0.001
  end
  
  def position(t)
    @curve.call(t)
  end
  
  def velocity(t)
    r_plus = position(t + @dt)
    r_minus = position(t - @dt)
    (r_plus - r_minus) * (1.0 / (2 * @dt))
  end
  
  def acceleration(t)
    v_plus = velocity(t + @dt)
    v_minus = velocity(t - @dt)
    (v_plus - v_minus) * (1.0 / (2 * @dt))
  end
  
  def speed(t)
    velocity(t).magnitude
  end
  
  def curvature(t)
    v = velocity(t)
    a = acceleration(t)
    
    cross_product = v.cross(a)
    cross_magnitude = cross_product.magnitude
    v_magnitude = v.magnitude
    
    return 0 if v_magnitude < 1e-10
    cross_magnitude / (v_magnitude**3)
  end
  
  def arc_length(t_start, t_end, num_points = 1000)
    length = 0
    step = (t_end - t_start) / num_points.to_f
    
    num_points.times do |i|
      t1 = t_start + i * step
      t2 = t_start + (i + 1) * step
      r1 = position(t1)
      r2 = position(t2)
      length += (r2 - r1).magnitude
    end
    
    length
  end
  
  def save_curve_data(t_start, t_end, num_points, filename)
    File.open(filename, 'w') do |file|
      file.puts "t,x,y,z,vx,vy,vz,speed,curvature"
      
      step = (t_end - t_start) / (num_points - 1).to_f
      
      num_points.times do |i|
        t = t_start + i * step
        r = position(t)
        v = velocity(t)
        k = curvature(t)
        
        file.puts "#{t},#{r.x},#{r.y},#{r.z},#{v.x},#{v.y},#{v.z},#{v.magnitude},#{k}"
      end
    end
    
    puts "Curve data written to #{filename}"
  end
  
  def analyze_properties(t_values)
    puts "Parameter\tPosition\t\tVelocity\t\tSpeed\t\tCurvature"
    puts "-" * 80
    
    t_values.each do |t|
      r = position(t)
      v = velocity(t)
      s = speed(t)
      k = curvature(t)
      
      puts "t=#{t.round(3)}\t\t#{r}\t#{v}\t#{s.round(3)}\t#{k.round(6)}"
    end
  end
end

# Define parametric curves
helix = ParametricCurve.new do |t|
  a, b = 1.0, 0.5
  Vector3D.new(a * Math.cos(t), a * Math.sin(t), b * t)
end

lissajous = ParametricCurve.new do |t|
  a, b, A, B, delta = 3, 2, 1, 1, Math::PI/2
  Vector3D.new(A * Math.sin(a * t + delta), B * Math.sin(b * t), 0)
end

cycloid = ParametricCurve.new do |t|
  R = 1
  Vector3D.new(R * (t - Math.sin(t)), R * (1 - Math.cos(t)), 0)
end

torus_knot = ParametricCurve.new do |t|
  R, r, p, q = 2, 1, 3, 2
  x = (R + r * Math.cos(q * t)) * Math.cos(p * t)
  y = (R + r * Math.cos(q * t)) * Math.sin(p * t)
  z = r * Math.sin(q * t)
  Vector3D.new(x, y, z)
end

viviani_curve = ParametricCurve.new do |t|
  a = 1
  Vector3D.new(a * (1 + Math.cos(t)), a * Math.sin(t), 2 * a * Math.sin(t/2))
end

puts "Parametric Curves Analysis (Ruby version)"
puts "=========================================="
puts

# Example 1: Helix
puts "Example 1: Helix"
puts "================"
t_point = Math::PI

r = helix.position(t_point)
v = helix.velocity(t_point)
a = helix.acceleration(t_point)
arc_len = helix.arc_length(0, t_point)
k = helix.curvature(t_point)

puts "At t = π:"
puts "Position: #{r}"
puts "Velocity: #{v}"
puts "Speed: #{v.magnitude.round(3)}"
puts "Acceleration: #{a}"
puts "Arc length from 0 to π: #{arc_len.round(3)}"
puts "Curvature: #{k.round(6)}"

helix.save_curve_data(0, 4*Math::PI, 200, "helix_ruby.csv")

# Theoretical verification for helix
a_helix, b_helix = 1.0, 0.5
theoretical_curvature = a_helix / (a_helix**2 + b_helix**2)
puts "Theoretical curvature for helix: #{theoretical_curvature.round(6)}"

# Example 2: Lissajous curve
puts "\n" + "="*50
puts "Example 2: Lissajous Curve (3:2 ratio)"
puts "======================================"

t_point = Math::PI/4
r_liss = lissajous.position(t_point)
v_liss = lissajous.velocity(t_point)

puts "At t = π/4:"
puts "Position: #{r_liss}"
puts "Velocity: #{v_liss}"
puts "Speed: #{v_liss.magnitude.round(3)}"

lissajous.save_curve_data(0, 2*Math::PI, 500, "lissajous_ruby.csv")

# Example 3: Cycloid
puts "\n" + "="*50
puts "Example 3: Cycloid"
puts "=================="

t_point = Math::PI
r_cyc = cycloid.position(t_point)
v_cyc = cycloid.velocity(t_point)
cycloid_arc = cycloid.arc_length(0, 2*Math::PI)

puts "At t = π:"
puts "Position: #{r_cyc}"
puts "Velocity: #{v_cyc}"
puts "Speed: #{v_cyc.magnitude.round(3)}"
puts "Arc length for one arch: #{cycloid_arc.round(3)}"
puts "Theoretical arc length: #{8} (8R for R=1)"

cycloid.save_curve_data(0, 4*Math::PI, 1000, "cycloid_ruby.csv")

# Example 4: Torus Knot
puts "\n" + "="*50
puts "Example 4: Torus Knot (3,2)"
puts "==========================="

t_point = 0
r_knot = torus_knot.position(t_point)
v_knot = torus_knot.velocity(t_point)
knot_length = torus_knot.arc_length(0, 2*Math::PI)

puts "At t = 0:"
puts "Position: #{r_knot}"
puts "Velocity: #{v_knot}"
puts "Speed: #{v_knot.magnitude.round(3)}"
puts "Total arc length: #{knot_length.round(3)}"

torus_knot.save_curve_data(0, 2*Math::PI, 1000, "torus_knot_ruby.csv")

# Example 5: Viviani's curve
puts "\n" + "="*50
puts "Example 5: Viviani's Curve"
puts "=========================="

viviani_curve.save_curve_data(0, 4*Math::PI, 500, "viviani_ruby.csv")

# Curvature analysis
puts "\n" + "="*50
puts "Curvature Analysis Comparison"
puts "============================="

test_points = [0, Math::PI/4, Math::PI/2, 3*Math::PI/4, Math::PI]

puts "\nHelix Curvature Analysis:"
helix.analyze_properties(test_points)

puts "\nLissajous Curvature Analysis:"
lissajous.analyze_properties(test_points)

# Special properties and comparisons
puts "\n" + "="*50
puts "Special Properties"
puts "=================="

curves = {
  "Helix" => helix,
  "Lissajous" => lissajous,
  "Cycloid" => cycloid,
  "Torus Knot" => torus_knot,
  "Viviani" => viviani_curve
}

curves.each do |name, curve|
  puts "\n#{name}:"
  length = curve.arc_length(0, 2*Math::PI, 500)
  max_curvature = 0
  max_speed = 0
  
  20.times do |i|
    t = i * 2 * Math::PI / 19
    k = curve.curvature(t)
    s = curve.speed(t)
    max_curvature = k if k > max_curvature
    max_speed = s if s > max_speed
  end
  
  puts "  Arc length (0 to 2π): #{length.round(3)}"
  puts "  Maximum curvature: #{max_curvature.round(6)}"
  puts "  Maximum speed: #{max_speed.round(3)}"
end

# Mathematical insights
puts "\n" + "="*50
puts "Mathematical Insights"
puts "===================="

puts "\n1. Helix Properties:"
puts "   - Constant curvature: #{theoretical_curvature.round(6)}"
puts "   - Constant torsion (for 3D helices)"
puts "   - Geodesic on cylinders"

puts "\n2. Cycloid Properties:"
puts "   - Brachistochrone curve (fastest descent)"
puts "   - Tautochrone curve (equal time property)"
puts "   - Generated by rolling circle"

puts "\n3. Lissajous Properties:"
puts "   - Closed curve when frequency ratio is rational"
puts "   - Phase difference affects shape"
puts "   - Applications in oscilloscopes"

puts "\n4. Torus Knot Properties:"
puts "   - Lies on torus surface"
puts "   - Knot invariants depend on (p,q) parameters"
puts "   - Non-trivial topology"

puts "\nNote: All CSV files contain detailed parameter data for visualization."