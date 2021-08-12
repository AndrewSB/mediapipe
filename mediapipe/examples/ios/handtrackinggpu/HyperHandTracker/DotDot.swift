//
//  DotDot.swift
//  HyperHandTracker
//
//  Created by Jaykumar Patel on 25/07/21.
//

infix operator ..: MultiplicationPrecedence

@discardableResult
public func .. <T>(object: T, block: (inout T) -> Void) -> T {
  var object = object
  block(&object)
  return object
}
